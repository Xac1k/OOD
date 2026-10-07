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

    int trigger = 0;
    auto subs = shape.SubscribeOnColorChanged([&trigger](Color color) {
        trigger++;
    });

    shape.SetColor(Blue);
    REQUIRE(trigger == 1);

    shape.MoveShape(1.0, 2.0);
    REQUIRE(trigger == 1);
}

TEST_CASE("Изменение фигуры, принадлежащей Picture, уведомляет наблюдателя Picture") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    int trigger = 0;
    auto subs = picture->SubscribeOnColorChanged([&trigger](
        const std::string& ID,
        Color color
    ) {
        trigger++;
    });
    shape.SetColor(Green);
    REQUIRE(trigger == 1);
}

TEST_CASE("Изменение Shape вызывает событие ShapeMoved у Picture") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    int count = 0;
    auto sub = picture->SubscribeOnShapeMoved([&](const std::string&, double, double) {
        count++;
    });

    shape.MoveShape(5.0, 5.0);

    REQUIRE(count == 1);
}

TEST_CASE("Добавление и удаление фигуры уведомляют наблюдателей Picture") {
    auto picture = MakePicture();

    int addCount = 0;
    int removeCount = 0;
    size_t lastAddShapesCount = 0;
    size_t lastRemoveShapesCount = 0;

    auto addSub = picture->SubscribeOnShapeAdded([&](const std::string&) {
        addCount++;
        lastAddShapesCount = picture->GetShapesCount();
    });
    auto remSub = picture->SubscribeOnShapeDeleted([&](const std::string&) {
        removeCount++;
        lastRemoveShapesCount = picture->GetShapesCount();
    });

    AddStubShape(*picture, "s1");
    REQUIRE(addCount == 1);
    REQUIRE(lastAddShapesCount == 1);

    picture->DeleteShape("s1");
    REQUIRE(removeCount == 1);
    REQUIRE(lastRemoveShapesCount == 0);
}

TEST_CASE("Смена цвета фигуры вызывает событие ColorChanged у Picture") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    int count = 0;
    auto sub = picture->SubscribeOnColorChanged([&](const std::string&, Color) {
        count++;
    });

    shape.SetColor(Green);

    REQUIRE(count == 1);
}

TEST_CASE("Удаление фигуры не вызывает событие изменения") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");

    int movedCount = 0;
    int colorCount = 0;
    auto moveSub = picture->SubscribeOnShapeMoved([&](const std::string&, double, double) {
        movedCount++;
    });
    auto colorSub = picture->SubscribeOnColorChanged([&](const std::string&, Color) {
        colorCount++;
    });

    picture->DeleteShape("s1");
    REQUIRE(movedCount == 0);
    REQUIRE(colorCount == 0);

    REQUIRE_THROWS_AS(picture->DeleteShape("s1"), ShapeExistenceException);
    REQUIRE(movedCount == 0);
    REQUIRE(colorCount == 0);
}

TEST_CASE("Несколько подписчиков одного события получают уведомления") {
    auto picture = MakePicture();

    int c1 = 0, c2 = 0, c3 = 0;
    auto sub1 = picture->SubscribeOnShapeAdded([&](const std::string&) { c1++; });
    auto sub2 = picture->SubscribeOnShapeAdded([&](const std::string&) { c2++; });
    auto sub3 = picture->SubscribeOnShapeAdded([&](const std::string&) { c3++; });

    AddStubShape(*picture, "s1");

    REQUIRE(c1 == 1);
    REQUIRE(c2 == 1);
    REQUIRE(c3 == 1);
}

TEST_CASE("После отписки подписчик не получает уведомления") {
    auto picture = MakePicture();

    int count = 0;
    auto sub = picture->SubscribeOnShapeAdded([&](const std::string&) {
        count++;
    });

    AddStubShape(*picture, "s1");
    REQUIRE(count == 1);

    sub.Disconnect();

    AddStubShape(*picture, "s2");
    REQUIRE(count == 1);
}

TEST_CASE("Повторная подписка одного и того же колбэка") {
    auto picture = MakePicture();

    int count = 0;
    auto callback = [&](const std::string&) { count++; };

    auto sub1 = picture->SubscribeOnShapeAdded(callback);
    auto sub2 = picture->SubscribeOnShapeAdded(callback);

    AddStubShape(*picture, "s1");

    // В новой событийной модели одинаковые лямбды не схлопываются автоматически.
    REQUIRE(count == 2);
}

TEST_CASE("Исключения при операциях с несуществующей фигурой") {
    auto picture = MakePicture();

    int removedCount = 0;
    int changedCount = 0;

    auto remSub = picture->SubscribeOnShapeDeleted([&](const std::string&) {
        removedCount++;
    });
    auto moveSub = picture->SubscribeOnShapeMoved([&](const std::string&, double, double) {
        changedCount++;
    });
    auto colorSub = picture->SubscribeOnColorChanged([&](const std::string&, Color) {
        changedCount++;
    });
    auto geomSub = picture->SubscribeOnShapeGeometryChanged([&](const std::string&, ShapeGeometry*) {
        changedCount++;
    });

    REQUIRE_THROWS_AS(picture->DeleteShape("nonexistent"), ShapeExistenceException);
    REQUIRE(removedCount == 0);
    REQUIRE(changedCount == 0);

    REQUIRE_THROWS_AS(picture->MoveShape("nonexistent", 1.0, 1.0), ShapeExistenceException);
    REQUIRE(changedCount == 0);

    REQUIRE_THROWS_AS(
        picture->ChangeShape("nonexistent", std::make_unique<StubGeometry>()),
        ShapeExistenceException);
    REQUIRE(changedCount == 0);
}

TEST_CASE("Изменение геометрии Shape вызывает событие ShapeGeometryChanged у Picture") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    int count = 0;
    auto sub = picture->SubscribeOnShapeGeometryChanged([&](const std::string&, ShapeGeometry*) {
        count++;
    });

    shape.SetGeometry(std::make_unique<StubGeometry>());

    REQUIRE(count == 1);
}

TEST_CASE("Изменение Shape вызывает события цвета, позиции и геометрии") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    int colorCount = 0;
    int posCount = 0;
    int geomCount = 0;

    auto colorSub = shape.SubscribeOnColorChanged([&](Color) { colorCount++; });
    auto posSub = shape.SubscribeOnPositionChanged([&](double, double) { posCount++; });
    auto geomSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) { geomCount++; });

    shape.SetColor(Blue);
    REQUIRE(colorCount == 1);

    shape.MoveShape(1.0, 1.0);
    REQUIRE(posCount == 1);

    shape.SetGeometry(std::make_unique<StubGeometry>());
    REQUIRE(geomCount == 1);
}

TEST_CASE("Клонирование Picture не копирует подписчиков") {
    auto picture = MakePicture();

    int c1 = 0, c2 = 0, c3 = 0;
    auto sub1 = picture->SubscribeOnShapeAdded([&](const std::string&) { c1++; });
    auto sub2 = picture->SubscribeOnShapeAdded([&](const std::string&) { c2++; });
    auto sub3 = picture->SubscribeOnShapeAdded([&](const std::string&) { c3++; });

    AddStubShape(*picture, "s1");

    REQUIRE(c1 == 1);
    REQUIRE(c2 == 1);
    REQUIRE(c3 == 1);

    StubCanvas newCanvas;
    auto clonedPicture = picture->Clone(std::make_unique<StubCanvas>(newCanvas));

    AddStubShape(clonedPicture, "s2");

    REQUIRE(c1 == 1);
    REQUIRE(c2 == 1);
    REQUIRE(c3 == 1);
}

TEST_CASE("Клонирование Picture с подписками") {
    auto picture = MakePicture();

    int c1 = 0, c2 = 0, c3 = 0;
    auto sub1 = picture->SubscribeOnShapeAdded([&](const std::string&) { c1++; });
    auto sub2 = picture->SubscribeOnShapeAdded([&](const std::string&) { c2++; });
    auto sub3 = picture->SubscribeOnShapeAdded([&](const std::string&) { c3++; });

    AddStubShape(*picture, "s1");

    StubCanvas newCanvas;
    auto clonedPicture = picture->Clone(std::make_unique<StubCanvas>(newCanvas));

    int c4 = 0, c5 = 0;
    auto sub4 = clonedPicture.SubscribeOnShapeAdded([&](const std::string&) { c4++; });
    auto sub5 = clonedPicture.SubscribeOnShapeAdded([&](const std::string&) { c5++; });

    AddStubShape(clonedPicture, "s2");

    REQUIRE(c1 == 1);
    REQUIRE(c2 == 1);
    REQUIRE(c3 == 1);
    REQUIRE(c4 == 1);
    REQUIRE(c5 == 1);
}

TEST_CASE("Удаление подписчика во время события Update Observer") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    using GeomSub = decltype(shape.SubscribeOnShapeGeometryChanged(
        std::function<void(ShapeGeometry*)>{}));

    std::optional<GeomSub> selfSub;
    int selfCount = 0;
    int geomCount = 0;

    selfSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        selfCount++;
        selfSub->Disconnect();
    });
    auto geomSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        geomCount++;
    });

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(selfCount == 1);
    REQUIRE(geomCount == 1);

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(selfCount == 1);
    REQUIRE(geomCount == 2);
}

TEST_CASE("Удаление другого подписчика во время события Update Observer") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    using GeomSub = decltype(shape.SubscribeOnShapeGeometryChanged(
        std::function<void(ShapeGeometry*)>{}));

    std::optional<GeomSub> geomSub;
    int deleteCount = 0;
    int geomCount = 0;

    auto deleteSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        deleteCount++;
        if (geomSub) {
            geomSub->Disconnect();
        }
    });
    geomSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        geomCount++;
    });

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(deleteCount == 1);
    REQUIRE(geomCount == 0);

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(deleteCount == 2);
    REQUIRE(geomCount == 0);
}

TEST_CASE("Добавление подписчика во время события Update Observer") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    using GeomSub = decltype(shape.SubscribeOnShapeGeometryChanged(
        std::function<void(ShapeGeometry*)>{}));

    std::optional<GeomSub> geomSub;
    int subscribeCount = 0;
    int geomCount = 0;

    auto subscribeSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        subscribeCount++;
        if (subscribeCount == 1) {
            geomSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
                geomCount++;
            });
        }
    });

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(subscribeCount == 1);
    REQUIRE(geomCount == 0);

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(subscribeCount == 2);
    REQUIRE(geomCount == 1);
}

TEST_CASE("Удаление подписчика и его повторная подписка во время события") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    using GeomSub = decltype(shape.SubscribeOnShapeGeometryChanged(
        std::function<void(ShapeGeometry*)>{}));

    std::optional<GeomSub> geomSub;
    int deleteCount = 0;
    int subscribeCount = 0;
    int geomCount = 0;

    auto deleteSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        deleteCount++;
        if (geomSub) {
            geomSub->Disconnect();
        }
    });
    geomSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        geomCount++;
    });
    auto subscribeSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
        subscribeCount++;
        if (subscribeCount == 1) {
            geomSub = shape.SubscribeOnShapeGeometryChanged([&](ShapeGeometry*) {
                geomCount++;
            });
        }
    });

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(deleteCount == 1);
    REQUIRE(subscribeCount == 1);
    REQUIRE(geomCount == 0);

    deleteSub.Disconnect();
    subscribeSub.Disconnect();

    REQUIRE_NOTHROW(shape.SetGeometry(std::make_unique<StubGeometry>()));
    REQUIRE(geomCount == 1);
}

// Эталонные тесты из условия

TEST_CASE("Изменение Shape вызывает событие только при изменении цвета") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    int trigger = 0;
    auto subs = shape.SubscribeOnColorChanged([&trigger](Color color) {
        trigger++;
    });

    shape.SetColor(Blue);
    REQUIRE(trigger == 1);

    shape.MoveShape(1.0, 2.0);
    REQUIRE(trigger == 1);
}

TEST_CASE("События фигуры, добавленной в Picture, приходят в Picture") {
    auto picture = MakePicture();
    AddStubShape(*picture, "s1");
    auto& shape = picture->GetShape("s1");

    int trigger = 0;
    auto subs = picture->SubscribeOnColorChanged([&trigger](
        const std::string& ID,
        Color color
    ) {
        trigger++;
    });

    shape.SetColor(Green);
    REQUIRE(trigger == 1);
}