#include "Triangle.h"

std::vector<std::string> shapes::Triangle::GetParams() {
    return {
        std::to_string(m_x1),
        std::to_string(m_y1),
        std::to_string(m_x2),
        std::to_string(m_y2),
        std::to_string(m_x3),
        std::to_string(m_y3),
    };
}

std::string shapes::Triangle::GetTypeName() {
    return "triangle";
}

void shapes::Triangle::MoveShape(double dx, double dy) {
    m_x1 += dx;
    m_y1 += dy;
    m_x2 += dx;
    m_y2 += dy;
    m_x3 += dx;
    m_y3 += dy;
}

std::unique_ptr<shapes::ShapeGeometry> shapes::Triangle::Clone() const {
    Triangle newTriangle(m_x1, m_y1, m_x2, m_y2, m_x3, m_y3);
    return std::make_unique<Triangle>(newTriangle);
}

void shapes::Triangle::Draw(gfx::ICanvas& canvas) {
    canvas.MoveTo(m_x1, m_y1);
    canvas.LineTo(m_x2, m_y2);
    canvas.LineTo(m_x3, m_y3);
    canvas.LineTo(m_x1, m_y1);
}

shapes::Triangle::Triangle(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3)
: m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_x3(x3), m_y3(y3)
{}

shapes::Triangle shapes::Triangle::CreateFromTokens(const std::vector<std::string>& tokens) {
    try {
        const auto x1 = std::stod(tokens.at(0));
        const auto y1 = std::stod(tokens.at(1));
        const auto x2 = std::stod(tokens.at(2));
        const auto y2 = std::stod(tokens.at(3));
        const auto x3 = std::stod(tokens.at(4));
        const auto y3 = std::stod(tokens.at(5));
        return {x1, y1, x2, y2, x3, y3};
    } catch(...) {
        throw std::runtime_error("[Triangle::CreateFromTokens] Parsing error. The format must be six numbers: <x1> <y1> <x2> <y2> <x3> <y3>.");
    }
}
