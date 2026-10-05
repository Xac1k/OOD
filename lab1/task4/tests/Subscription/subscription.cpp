#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <memory>
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


class OnGeometryChangedObserver : public Observer<Shape> {
public:
    int count = 0;
    void Update(const Shape&) override { ++count; }
};

class OnGeometryChangedObserverOweSub : public AutoObserver<Shape, Shape::EventType> {
public:
    explicit OnGeometryChangedObserverOweSub(Shape& shape)
    : AutoObserver(shape, Shape::EventType::GeometryChanged) {}
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

TEST_CASE("Пока подписка храниться, события приходят.") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");
    OnGeometryChangedObserver observer;

    {
        auto subscription = shape.Subscribe(Shape::EventType::GeometryChanged, observer);
        CHECK(subscription.IsActive() == true);

        shape.SetGeometry(std::make_unique<StubGeometry>());
        CHECK(observer.count == 1);
    }

    shape.SetGeometry(std::make_unique<StubGeometry>());
    CHECK(observer.count == 1);
}

TEST_CASE("Выход из scope + удаление подписки.") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");
    OnGeometryChangedObserver observer;

    {
        auto subscription = shape.Subscribe(Shape::EventType::GeometryChanged, observer);
        CHECK(subscription.IsActive() == true);
    }

    shape.SetGeometry(std::make_unique<StubGeometry>());
    CHECK(observer.count == 0);
}

TEST_CASE("Разрываем связь руками.") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));
    OnGeometryChangedObserver observer;

    auto sub = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);
    sub.Disconnect();

    stubShape.SetGeometry(std::unique_ptr<StubGeometry>{});
    CHECK(observer.count == 0);
    CHECK(sub.IsActive() == false);
}

TEST_CASE("Double Disconnect не падает с ошибкой.") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));
    OnGeometryChangedObserver observer;

    auto sub = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);
    sub.Disconnect();
    CHECK_NOTHROW(sub.Disconnect());

    stubShape.SetGeometry(std::unique_ptr<StubGeometry>{});
    CHECK(observer.count == 0);
    CHECK(sub.IsActive() == false);
}

TEST_CASE("Observer удаляет себя внутри update. Подписка должна прекратиться.") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));
    OnGeometryChangedUnsubscribeObserver observer(&stubShape);
    auto subscription = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);

    stubShape.SetGeometry(std::make_unique<StubGeometry>());
    CHECK(subscription.IsActive() == false);
}

TEST_CASE("Разрушение объекта подписки ведёт к протухшей подписке.") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));
    OnGeometryChangedObserver observer;

    auto subscription = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);
    stubShape.~Shape();

    CHECK(subscription.IsActive() == false);
}

TEST_CASE("Безопасный Disconnect после разрушения объекта подписики") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));
    OnGeometryChangedObserver observer;

    auto subscription = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);
    stubShape.~Shape();

    CHECK_NOTHROW(subscription.Disconnect());
}

TEST_CASE("Повторная слушание события даёт две связанные подписки.") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));
    OnGeometryChangedObserver observer;

    auto subscription1 = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);
    auto subscription2 = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);
    stubShape.~Shape();

    CHECK(subscription1.IsActive() == false);
    CHECK(subscription2.IsActive() == false);
}

TEST_CASE("Перемещение объекта подписки передаёт владение.") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));
    OnGeometryChangedObserver observer;
    Observable<Shape, Shape::EventType>::Subscription movedToSub{nullptr};

    {
        auto sub = stubShape.Subscribe(Shape::EventType::GeometryChanged, observer);
        movedToSub = std::move(sub);
    }

    CHECK(movedToSub.IsActive() == true);
    stubShape.SetGeometry(std::make_unique<StubGeometry>());
    CHECK(observer.count == 1);
    CHECK_NOTHROW(movedToSub.Disconnect());
    stubShape.SetGeometry(std::make_unique<StubGeometry>());
    CHECK(observer.count == 1);
}

TEST_CASE("Наблюдатель владеющий подпиской") {
    auto stubShape = Shape(std::make_unique<StubGeometry>(), Color(255, 0, 0));

    {
        OnGeometryChangedObserverOweSub observer(stubShape);
        stubShape.SetGeometry(std::make_unique<StubGeometry>());

        CHECK(observer.count == 1);
    }

    CHECK_NOTHROW(stubShape.SetGeometry(std::make_unique<StubGeometry>()));
}