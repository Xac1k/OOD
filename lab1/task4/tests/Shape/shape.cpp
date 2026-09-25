#include <catch2/catch_test_macros.hpp>
#include "Shape.h"
#include "Common/Color.h"
#include "ShapeGeometry/CRTPShapeGeometry.h"

class MockShapeGeometry : public shapes::ShapeGeometry {
public:
    MockShapeGeometry(std::string typeName, std::vector<std::string> params)
        : m_typeName(std::move(typeName)), m_params(std::move(params)) {}

    [[nodiscard]] std::string GetTypeName() override {
        return m_typeName;
    }

    [[nodiscard]] std::vector<std::string> GetParams() override {
        return m_params;
    }

    [[nodiscard]] std::unique_ptr<ShapeGeometry> Clone() const override {
        return std::make_unique<MockShapeGeometry>(m_typeName, m_params);
    }

    void MoveShape(double dx, double dy) override {
        m_moved = true;
        m_dx = dx;
        m_dy = dy;
    }

    void Draw(gfx::ICanvas&) override {
        ++m_drawCount;
    }

    bool WasMoved() const { return m_moved; }
    double Dx() const { return m_dx; }
    double Dy() const { return m_dy; }
    int m_drawCount = 0;

private:
    std::string m_typeName;
    std::vector<std::string> m_params;
    bool m_moved = false;
    double m_dx = 0.0;
    double m_dy = 0.0;
};

class MockCanvas : public gfx::ICanvas {
public:
    void MoveTo(double x, double y) override {};

    void SetColor(Color color) override {};

    void LineTo(double x, double y) override {};

    void DrawEllipse(double cx, double cy, double rx, double ry) override {};

    void DrawText(double left, double top, double fontSize, const std::string& text) override {};
};

namespace {
    std::unique_ptr<MockShapeGeometry> MakeGeometry(
        std::string type = "Circle",
        std::vector<std::string> params = {"10", "20", "5"})
    {
        return std::make_unique<MockShapeGeometry>(std::move(type), std::move(params));
    }
}

TEST_CASE("Shape: конструктор и базовые геттеры") {
    Color color(1, 2, 3);
    shapes::Shape shape(MakeGeometry("Circle", {"10", "20", "5"}), color);

    SECTION("GetTypeName возвращает имя типа геометрии") {
        REQUIRE(shape.GetTypeName() == "Circle");
    }

    SECTION("GetColor возвращает заданный цвет") {
        REQUIRE(shape.GetColor() == color);
    }

    SECTION("GetParams возвращает параметры геометрии") {
        auto params = shape.GetParams();
        REQUIRE(params.size() == 3);
        REQUIRE(params[0] == "10");
        REQUIRE(params[1] == "20");
        REQUIRE(params[2] == "5");
    }
}

TEST_CASE("Shape: SetColor") {
    shapes::Shape shape(MakeGeometry(), Color(0, 0, 0));

    Color newColor(255, 128, 64);
    shape.SetColor(newColor);

    REQUIRE(shape.GetColor() == newColor);
}

TEST_CASE("Shape: MoveShape делегирует вызов геометрии") {
    auto geometryPtr = MakeGeometry();
    auto* rawGeometry = geometryPtr.get();
    shapes::Shape shape(std::move(geometryPtr), Color(0, 0, 0));

    shape.MoveShape(5.0, -3.5);

    REQUIRE(rawGeometry->WasMoved());
    REQUIRE(rawGeometry->Dx() == 5.0);
    REQUIRE(rawGeometry->Dy() == -3.5);
}

TEST_CASE("Shape: SetGeometry заменяет геометрию") {
    shapes::Shape shape(MakeGeometry("Circle", {"1"}), Color(0, 0, 0));

    REQUIRE(shape.GetTypeName() == "Circle");
    auto params = shape.GetParams();
    REQUIRE(params.size() == 1);
    REQUIRE(params[0] == "1");

    shape.SetGeometry(MakeGeometry("Rectangle", {"10", "20"}));

    REQUIRE(shape.GetTypeName() == "Rectangle");
    auto params1 = shape.GetParams();
    REQUIRE(params1.size() == 2);
    REQUIRE(params1[0] == "10");
    REQUIRE(params1[1] == "20");
}

TEST_CASE("Shape: Draw вызывает Draw у геометрии") {
    auto geometryPtr = MakeGeometry();
    auto* rawGeometry = geometryPtr.get();
    shapes::Shape shape(std::move(geometryPtr), Color(0, 0, 0));

    MockCanvas canvas;
    shape.Draw(canvas);

    REQUIRE(rawGeometry->m_drawCount == 1);
}

TEST_CASE("Shape: Clone создаёт независимую копию") {
    Color color(10, 20, 30);
    shapes::Shape original(MakeGeometry("Triangle", {"1", "2", "3"}), color);

    auto clone = original.Clone();

    REQUIRE(clone != nullptr);
    REQUIRE(clone->GetTypeName() == original.GetTypeName());
    REQUIRE(clone->GetColor() == original.GetColor());
    REQUIRE(clone->GetParams() == original.GetParams());

    SECTION("Изменение клона не влияет на оригинал") {
        clone->SetColor(Color(0, 0, 0));
        REQUIRE(original.GetColor() == color);
        REQUIRE(clone->GetColor() != original.GetColor());
    }

    SECTION("Изменение геометрии клона не влияет на оригинал") {
        clone->SetGeometry(MakeGeometry("Square", {"7"}));
        REQUIRE(original.GetTypeName() == "Triangle");
        REQUIRE(clone->GetTypeName() == "Square");
    }

    SECTION("MoveShape на клоне не двигает оригинал") {
        auto geometryPtr = MakeGeometry();
        auto* rawGeometry = geometryPtr.get();
        shapes::Shape shape(std::move(geometryPtr), color);

        auto shapeClone = shape.Clone();
        shapeClone->MoveShape(100.0, 100.0);

        REQUIRE_FALSE(rawGeometry->WasMoved());
    }
}

TEST_CASE("Shape: GetTypeName после замены геометрии") {
    shapes::Shape shape(MakeGeometry("Circle"), Color(0, 0, 0));
    shape.SetGeometry(MakeGeometry("Hexagon"));

    REQUIRE(shape.GetTypeName() == "Hexagon");
}