#include <catch2/catch_test_macros.hpp>
#include "Picture.h"
#include "Shape.h"
#include "ShapeGeometry/CRTPShapeGeometry.h"

namespace shapes
{
    class Picture;
    class Shape;
}

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

    int DrawCount() { return m_drawCount; }
    int MoveCount() const { return m_moveCount; }
    bool WasMoved() const { return m_moved; }
    double Dx() const { return m_dx; }
    double Dy() const { return m_dy; }


private:
    int m_moveCount = 0;
    mutable int m_drawCount = 0;

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
        std::vector<std::string> params = {"10", "20", "5"}
    ) {
        return std::make_unique<MockShapeGeometry>(std::move(type), std::move(params));
    }

    std::unique_ptr<shapes::Shape> MakeShape(
        std::string type = "Circle",
        std::vector<std::string> params = {"10", "20", "5"},
        Color color = Color(0, 0, 0)
    ) {
        return std::make_unique<shapes::Shape>(MakeGeometry(std::move(type), std::move(params)), color);
    }

    struct PictureFixture {
        std::unique_ptr<MockCanvas> canvasOwned = std::make_unique<MockCanvas>();
        MockCanvas* canvas = canvasOwned.get();
        std::unique_ptr<shapes::Picture> picture = std::make_unique<shapes::Picture>(std::move(canvasOwned));
    };
}

TEST_CASE("Picture: AddShape добавляет фигуру") {
    PictureFixture f;

    REQUIRE_NOTHROW(f.picture->AddShape("id1", MakeShape("Circle")));

    auto list = f.picture->List();
    REQUIRE(list.find("id1") != std::string::npos);
}

TEST_CASE("Picture: AddShape с дублирующимся ID бросает исключение") {
    PictureFixture f;
    f.picture->AddShape("id1", MakeShape("Circle"));

    REQUIRE_THROWS_AS(
        f.picture->AddShape("id1", MakeShape("Rectangle")),
        shapes::ShapeExistenceException
    );
}

TEST_CASE("Picture: DeleteShape удаляет фигуру") {
    PictureFixture f;
    f.picture->AddShape("id1", MakeShape("Circle"));

    REQUIRE_NOTHROW(f.picture->DeleteShape("id1"));
    REQUIRE(f.picture->List().find("id1") == std::string::npos);
}

TEST_CASE("Picture: DeleteShape для несуществующего ID бросает исключение") {
    PictureFixture f;

    REQUIRE_THROWS_AS(
        f.picture->DeleteShape("missing"),
        shapes::ShapeExistenceException
    );
}

TEST_CASE("Picture: MoveShape делегирует Move в геометрию") {
    PictureFixture f;

    auto geometry = MakeGeometry("Circle");
    auto* rawGeometry = geometry.get();

    f.picture->AddShape("id1", std::make_unique<shapes::Shape>(std::move(geometry), Color(0, 0, 0)));
    f.picture->MoveShape("id1", 7.0, -4.0);

    REQUIRE(rawGeometry->MoveCount() == 1);
    REQUIRE(rawGeometry->Dx() == 7.0);
    REQUIRE(rawGeometry->Dy() == -4.0);
}

TEST_CASE("Picture: MoveShape для несуществующего ID бросает исключение") {
    PictureFixture f;

    REQUIRE_THROWS_AS(
        f.picture->MoveShape("missing", 1.0, 1.0),
        shapes::ShapeExistenceException
    );
}

TEST_CASE("Picture: MovePicture сдвигает все фигуры") {
    PictureFixture f;

    auto g1 = MakeGeometry("Circle");
    auto g2 = MakeGeometry("Rectangle");
    auto* raw1 = g1.get();
    auto* raw2 = g2.get();

    f.picture->AddShape("a", std::make_unique<shapes::Shape>(std::move(g1), Color(0, 0, 0)));
    f.picture->AddShape("b", std::make_unique<shapes::Shape>(std::move(g2), Color(0, 0, 0)));

    f.picture->MovePicture(3.0, 5.0);

    REQUIRE(raw1->MoveCount() == 1);
    REQUIRE(raw2->MoveCount() == 1);
    REQUIRE(raw1->Dx() == 3.0);
    REQUIRE(raw2->Dy() == 5.0);
}

TEST_CASE("Picture: MovePicture noexcept") {
    PictureFixture f;

    REQUIRE_NOTHROW(f.picture->MovePicture(1.0, 1.0));

    f.picture->AddShape("a", MakeShape());
    REQUIRE_NOTHROW(f.picture->MovePicture(1.0, 1.0));
}

TEST_CASE("Picture: DrawPicture рисует все фигуры") {
    PictureFixture f;

    auto g1 = MakeGeometry();
    auto g2 = MakeGeometry();
    auto* raw1 = g1.get();
    auto* raw2 = g2.get();

    f.picture->AddShape("a", std::make_unique<shapes::Shape>(std::move(g1), Color(0, 0, 0)));
    f.picture->AddShape("b", std::make_unique<shapes::Shape>(std::move(g2), Color(0, 0, 0)));

    f.picture->DrawPicture();

    REQUIRE(raw1->DrawCount() == 1);
    REQUIRE(raw2->DrawCount() == 1);
}

TEST_CASE("Picture: DrawPicture на пустой картинке noexcept") {
    PictureFixture f;

    REQUIRE_NOTHROW(f.picture->DrawPicture());
}

TEST_CASE("Picture: DrawShape рисует только указанную фигуру") {
    PictureFixture f;

    auto g1 = MakeGeometry();
    auto g2 = MakeGeometry();
    auto* raw1 = g1.get();
    auto* raw2 = g2.get();

    f.picture->AddShape("a", std::make_unique<shapes::Shape>(std::move(g1), Color(0, 0, 0)));
    f.picture->AddShape("b", std::make_unique<shapes::Shape>(std::move(g2), Color(0, 0, 0)));

    f.picture->DrawShape("a");

    REQUIRE(raw1->DrawCount() == 1);
    REQUIRE(raw2->DrawCount() == 0);
}

TEST_CASE("Picture: DrawShape для несуществующего ID бросает исключение") {
    PictureFixture f;

    REQUIRE_THROWS_AS(
        f.picture->DrawShape("missing"),
        shapes::ShapeExistenceException
    );
}

TEST_CASE("Picture: ChangeColor меняет цвет фигуры") {
    PictureFixture f;
    Color initial(10, 20, 30);
    f.picture->AddShape("id1", MakeShape("Circle", {"1"}, initial));

    Color newColor(200, 100, 50);
    REQUIRE_NOTHROW(f.picture->ChangeColor("id1", newColor));
}

TEST_CASE("Picture: ChangeColor для несуществующего ID бросает исключение") {
    PictureFixture f;

    REQUIRE_THROWS_AS(
        f.picture->ChangeColor("missing", Color(0, 0, 0)),
        shapes::ShapeExistenceException
    );
}

TEST_CASE("Picture: ChangeShape заменяет геометрию") {
    PictureFixture f;
    f.picture->AddShape("id1", MakeShape("Circle", {"1"}));

    REQUIRE_NOTHROW(f.picture->ChangeShape("id1", MakeGeometry("Rectangle", {"10", "20"})));
}

TEST_CASE("Picture: ChangeShape для несуществующего ID бросает исключение") {
    PictureFixture f;

    REQUIRE_THROWS_AS(
        f.picture->ChangeShape("missing", MakeGeometry()),
        shapes::ShapeExistenceException);
}

TEST_CASE("Picture: CloneShape создаёт независимую копию") {
    PictureFixture f;
    auto original = MakeShape("Circle", {"1"}, Color(1, 2, 3));
    auto* originalPtr = original.get();
    f.picture->AddShape("src", std::move(original));

    REQUIRE_NOTHROW(f.picture->CloneShape("src", "dst"));

    auto list = f.picture->List();
    REQUIRE(list.find("src") != std::string::npos);
    REQUIRE(list.find("dst") != std::string::npos);

    Color clr(99, 99, 99);
    f.picture->ChangeColor("dst", clr);
    REQUIRE(originalPtr->GetColor() != clr);
}

TEST_CASE("Picture: CloneShape с существующим newID бросает исключение") {
    PictureFixture f;
    f.picture->AddShape("src", MakeShape());
    f.picture->AddShape("dst", MakeShape());

    REQUIRE_THROWS_AS(
        f.picture->CloneShape("src", "dst"),
        shapes::ShapeExistenceException
    );
}

TEST_CASE("Picture: CloneShape с несуществующим srcID бросает исключение") {
    PictureFixture f;

    REQUIRE_THROWS_AS(
        f.picture->CloneShape("missing", "new"),
        shapes::ShapeExistenceException
    );
}

TEST_CASE("Picture: List содержит все добавленные ID") {
    PictureFixture f;
    f.picture->AddShape("alpha", MakeShape());
    f.picture->AddShape("beta",  MakeShape());
    f.picture->AddShape("gamma", MakeShape());

    auto list = f.picture->List();
    REQUIRE(list.find("alpha") != std::string::npos);
    REQUIRE(list.find("beta")  != std::string::npos);
    REQUIRE(list.find("gamma") != std::string::npos);
}

TEST_CASE("Picture: List пустой для пустой картинки") {
    PictureFixture f;

    REQUIRE(f.picture->List().empty());
}

TEST_CASE("Picture: List не содержит удалённые ID") {
    PictureFixture f;
    f.picture->AddShape("keep", MakeShape());
    f.picture->AddShape("drop", MakeShape());

    f.picture->DeleteShape("drop");

    auto list = f.picture->List();
    REQUIRE(list.find("keep") != std::string::npos);
    REQUIRE(list.find("drop") == std::string::npos);
}