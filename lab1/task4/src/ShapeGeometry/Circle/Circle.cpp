#include "Circle.h"

std::vector<std::string> shapes::Circle::GetParams() {
    return {std::to_string(m_x), std::to_string(m_y), std::to_string(m_radius)};
}

std::string shapes::Circle::GetTypeName() {
    return "circle";
}

shapes::Circle::Circle(const double x, const double y, const double radius)
: m_x(x), m_y(y), m_radius(radius)
{}

void shapes::Circle::MoveShape(const double dx, const double dy) {
    m_x += dx;
    m_y += dy;
}

std::unique_ptr<shapes::ShapeGeometry> shapes::Circle::Clone() const {
    return std::make_unique<Circle>(*this);
}

void shapes::Circle::Draw(gfx::ICanvas& canvas) {
    canvas.DrawEllipse(m_x, m_y, m_radius, m_radius);
}

shapes::Circle shapes::Circle::CreateFromTokens(const std::vector<std::string>& tokens) {
    try {
        const auto x = std::stod(tokens.at(0));
        const auto y = std::stod(tokens.at(1));
        const auto radius = std::stod(tokens.at(2));
        return {x, y, radius};
    } catch(...) {
        throw std::runtime_error("[Circle::CreateFromTokens] Parsing error. The format must be three numbers: <x> <y> <radius>.");
    }
}
