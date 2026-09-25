#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Picture.h"
#include "Shape.h"
#include "Common/Observer.h"
#include "ShapeGeometry/CRTPShapeGeometry.h"
#include "Canvas/ICanvas.h"

using namespace shapes;

class StubGeometry : public ShapeGeometry {
public:
    void Draw(gfx::ICanvas& canvas) override {};
    std::vector<std::string> GetParams() override { return {}; };
    std::string GetTypeName() override {
        return "stub";
    };
    void MoveShape(double dx, double dy) override {};

    [[nodiscard]] std::unique_ptr<ShapeGeometry> Clone() const override {
        return std::make_unique<StubGeometry>(*this);
    }
};

class StubCanvas : public gfx::ICanvas {
public:
    void DrawEllipse(double cx, double cy, double rx, double ry) override {};
    void DrawText(double left, double top, double fontSize, const std::string& text) override {};
    void LineTo(double x, double y) override {};
    void MoveTo(double x, double y) override {};
    void SetColor(Color color) override {};
};

class CountingPictureObserver : public Observer<Picture> {
public:
    int shapeChanged = 0;
    int shapeAdded = 0;
    int shapeRemoved = 0;
    size_t lastShapesCount = 0;

    void Update(const Picture& data) override {
        lastShapesCount = data.GetShapesCount();
    }
};

class OnShapeChangedObserver : public Observer<Picture> {
public:
    int count = 0;
    void Update(const Picture&) override { ++count; }
};
class OnShapeAddedObserver : public Observer<Picture> {
public:
    int count = 0;
    size_t lastShapesCount = 0;
    void Update(const Picture& data) override { ++count; lastShapesCount = data.GetShapesCount(); }
};
class OnShapeRemovedObserver : public Observer<Picture> {
public:
    int count = 0;
    size_t lastShapesCount = 0;
    void Update(const Picture& data) override { ++count; lastShapesCount = data.GetShapesCount(); }
};

class OnColorChangedObserver : public Observer<Shape> {
public:
    int count = 0;
    void Update(const Shape&) override { ++count; }
};
class OnPositionChangedObserver : public Observer<Shape> {
public:
    int count = 0;
    void Update(const Shape&) override { ++count; }
};
class OnGeometryChangedObserver : public Observer<Shape> {
public:
    int count = 0;
    void Update(const Shape&) override { ++count; }
};
class OnGeometryChangedUnsubscribeObserver : public Observer<Shape> {
public:
    explicit OnGeometryChangedUnsubscribeObserver(Shape* shape) : m_shape(shape) {};
    int count = 0;
    void Update(const Shape& data) override {
        count++;
        m_shape->Unsubscribe(Shape::EventType::GeometryChanged, *this);
    }

private:
    Shape* m_shape;
};

const Color Blue = {0, 0, 255};
const Color Red = {255, 0, 0};
const Color Green = {0, 255, 0};

namespace {

    std::unique_ptr<Picture> MakePicture() {
        return std::make_unique<Picture>(std::make_unique<StubCanvas>());
    }

    void AddStubShape(Picture& picture, const std::string& id) {
        picture.AddShape(id, std::make_unique<Shape>(
            std::make_unique<StubGeometry>(), Color(255, 0, 0)));
    }
}


TEST_CASE("Наблюдатель Shape получает уведомление после изменения фигуры") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    OnColorChangedObserver colorObs;
    OnPositionChangedObserver posObs;
    shape.Subscribe(Shape::EventType::ColorChanged, colorObs);
    shape.Subscribe(Shape::EventType::PositionChanged, posObs);

    shape.SetColor(Blue);
    REQUIRE(colorObs.count == 1);

    shape.MoveShape(1.0, 2.0);
    REQUIRE(posObs.count == 1);
}

TEST_CASE("Изменение фигуры, принадлежащей Picture, уведомляет наблюдателя Picture") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    OnShapeChangedObserver observer;
    picture->Subscribe(Picture::EventType::ShapeChanged, observer);

    shape.SetColor(Green);

    REQUIRE(observer.count == 1);
}

TEST_CASE("Изменение Shape через прямую ссылку уведомляет наблюдателя Picture") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    OnShapeChangedObserver observer;
    picture->Subscribe(Picture::EventType::ShapeChanged, observer);

    shape.MoveShape(5.0, 5.0);

    REQUIRE(observer.count == 1);
}

TEST_CASE("Добавление и удаление фигуры уведомляет наблюдателей Picture") {
    auto picture = MakePicture();

    OnShapeAddedObserver addObs;
    OnShapeRemovedObserver remObs;
    picture->Subscribe(Picture::EventType::ShapeAdded, addObs);
    picture->Subscribe(Picture::EventType::ShapeRemoved, remObs);

    AddStubShape(*picture, "s1");
    REQUIRE(addObs.count == 1);
    REQUIRE(addObs.lastShapesCount == 1);

    picture->DeleteShape("s1");
    REQUIRE(remObs.count == 1);
    REQUIRE(remObs.lastShapesCount == 0);
}

TEST_CASE("После добавления новой фигуры Picture начинает наблюдать за ней") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    OnShapeChangedObserver observer;
    picture->Subscribe(Picture::EventType::ShapeChanged, observer);

    shape.SetColor(Green);

    REQUIRE(observer.count == 1);
}

TEST_CASE("После удаления фигуры Picture больше не имеет подписки на неё") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");

    OnShapeChangedObserver observer;
    picture->Subscribe(Picture::EventType::ShapeChanged, observer);

    picture->DeleteShape("s1");
    REQUIRE(observer.count == 0);

    REQUIRE_THROWS_AS(picture->DeleteShape("s1"), ShapeExistenceException);
    REQUIRE(observer.count == 0);
}

TEST_CASE("Несколько наблюдателей одного субъекта получают уведомления") {
    auto picture = MakePicture();

    OnShapeAddedObserver o1, o2, o3;
    picture->Subscribe(Picture::EventType::ShapeAdded, o1);
    picture->Subscribe(Picture::EventType::ShapeAdded, o2);
    picture->Subscribe(Picture::EventType::ShapeAdded, o3);

    AddStubShape(*picture, "s1");

    REQUIRE(o1.count == 1);
    REQUIRE(o2.count == 1);
    REQUIRE(o3.count == 1);
}

TEST_CASE("После отписки наблюдатель перестаёт получать уведомления") {
    auto picture = MakePicture();

    OnShapeAddedObserver observer;
    picture->Subscribe(Picture::EventType::ShapeAdded, observer);

    AddStubShape(*picture, "s1");
    REQUIRE(observer.count == 1);

    picture->Unsubscribe(Picture::EventType::ShapeAdded, observer);

    AddStubShape(*picture, "s2");
    REQUIRE(observer.count == 1);
}

TEST_CASE("Повторная регистрация наблюдателя не приводит к дублированию уведомлений") {
    auto picture = MakePicture();

    OnShapeAddedObserver observer;
    picture->Subscribe(Picture::EventType::ShapeAdded, observer);
    picture->Subscribe(Picture::EventType::ShapeAdded, observer);

    AddStubShape(*picture, "s1");

    REQUIRE(observer.count == 1);
}

TEST_CASE("Неуспешная операция, не изменившая состояние, не уведомляет") {
    auto picture = MakePicture();

    OnShapeRemovedObserver remObs;
    OnShapeChangedObserver chgObs;
    picture->Subscribe(Picture::EventType::ShapeRemoved, remObs);
    picture->Subscribe(Picture::EventType::ShapeChanged, chgObs);

    REQUIRE_THROWS_AS(picture->DeleteShape("nonexistent"), ShapeExistenceException);
    REQUIRE(remObs.count == 0);
    REQUIRE(chgObs.count == 0);

    REQUIRE_THROWS_AS(picture->MoveShape("nonexistent", 1.0, 1.0), ShapeExistenceException);
    REQUIRE(chgObs.count == 0);

    REQUIRE_THROWS_AS(
        picture->ChangeShape("nonexistent", std::make_unique<StubGeometry>()),
        ShapeExistenceException);
    REQUIRE(chgObs.count == 0);
}

TEST_CASE("Наблюдатель Picture получает уведомление при изменении геометрии фигуры") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    OnShapeChangedObserver observer;
    picture->Subscribe(Picture::EventType::ShapeChanged, observer);

    shape.SetGeometry(std::make_unique<StubGeometry>());

    REQUIRE(observer.count == 1);
}

TEST_CASE("Наблюдатель Shape получает уведомление при изменении цвета, позиции и геометрии") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    OnColorChangedObserver colorObs;
    OnPositionChangedObserver posObs;
    OnGeometryChangedObserver geomObs;

    shape.Subscribe(Shape::EventType::ColorChanged, colorObs);
    shape.Subscribe(Shape::EventType::PositionChanged, posObs);
    shape.Subscribe(Shape::EventType::GeometryChanged, geomObs);

    shape.SetColor(Blue);
    REQUIRE(colorObs.count == 1);

    shape.MoveShape(1.0, 1.0);
    REQUIRE(posObs.count == 1);

    shape.SetGeometry(std::make_unique<StubGeometry>());
    REQUIRE(geomObs.count == 1);
}

TEST_CASE("Отписка от наблюдения за объектом внутри Update Observer") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    OnGeometryChangedUnsubscribeObserver deletedOnUpdate(&shape);
    OnGeometryChangedObserver geomObs;
    shape.Subscribe(Shape::EventType::GeometryChanged, deletedOnUpdate);
    shape.Subscribe(Shape::EventType::GeometryChanged, geomObs);

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(deletedOnUpdate.count == 1);
    REQUIRE(geomObs.count == 1);

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(deletedOnUpdate.count == 1);
    REQUIRE(geomObs.count == 2);
}

TEST_CASE("Копируется Picture, но не Observers") {
    auto picture = MakePicture();

    OnShapeAddedObserver o1, o2, o3;
    picture->Subscribe(Picture::EventType::ShapeAdded, o1);
    picture->Subscribe(Picture::EventType::ShapeAdded, o2);
    picture->Subscribe(Picture::EventType::ShapeAdded, o3);

    AddStubShape(*picture, "s1");

    REQUIRE(o1.count == 1);
    REQUIRE(o2.count == 1);
    REQUIRE(o3.count == 1);


    StubCanvas newCanvas;
    auto clonedPicture = picture->Clone(std::make_unique<StubCanvas>(newCanvas));

    AddStubShape(clonedPicture, "s2");

    REQUIRE(o1.count == 1);
    REQUIRE(o2.count == 1);
    REQUIRE(o3.count == 1);
}

TEST_CASE("Копирование Picture и подписка на копию") {
    auto picture = MakePicture();

    OnShapeAddedObserver o1, o2, o3;
    picture->Subscribe(Picture::EventType::ShapeAdded, o1);
    picture->Subscribe(Picture::EventType::ShapeAdded, o2);
    picture->Subscribe(Picture::EventType::ShapeAdded, o3);

    AddStubShape(*picture, "s1");

    StubCanvas newCanvas;
    auto clonedPicture = picture->Clone(std::make_unique<StubCanvas>(newCanvas));

    OnShapeAddedObserver o4, o5;
    clonedPicture.Subscribe(Picture::EventType::ShapeAdded, o4);
    clonedPicture.Subscribe(Picture::EventType::ShapeAdded, o5);
    AddStubShape(clonedPicture, "s2");

    REQUIRE(o1.count == 1);
    REQUIRE(o2.count == 1);
    REQUIRE(o3.count == 1);
    REQUIRE(o4.count == 1);
    REQUIRE(o5.count == 1);
}
