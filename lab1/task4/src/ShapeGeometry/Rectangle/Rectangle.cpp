#include "Rectangle.h"

std::vector<std::string> shapes::Rectangle::GetParams() {
    return {
        std::to_string(m_left),
        std::to_string(m_top),
        std::to_string(m_width),
        std::to_string(m_height)
    };
}

std::string shapes::Rectangle::GetTypeName() {
    return "rectangle";
}

void shapes::Rectangle::MoveShape(const double dx, const double dy) {
    m_left += dx;
    m_top += dy;
}

std::unique_ptr<shapes::ShapeGeometry> shapes::Rectangle::Clone() const {
    Rectangle newRectangle(m_left, m_top, m_width, m_height);

    return std::make_unique<Rectangle>(newRectangle);
}

void shapes::Rectangle::Draw(gfx::ICanvas& canvas) {
    canvas.MoveTo(m_left, m_top);
    canvas.LineTo(m_left + m_width, m_top);
    canvas.LineTo(m_left + m_width, m_top + m_height);
    canvas.LineTo(m_left, m_top + m_height);
    canvas.LineTo(m_left, m_top);
}

shapes::Rectangle::Rectangle(const double left, const double top, const double width, const double height)
: m_left(left), m_top(top), m_width(width), m_height(height)
{}

shapes::Rectangle shapes::Rectangle::CreateFromTokens(const std::vector<std::string>& tokens) {
    try {
        const auto left = std::stod(tokens.at(0));
        const auto top = std::stod(tokens.at(1));
        const auto width = std::stod(tokens.at(2));
        const auto height = std::stod(tokens.at(3));
        return {left, top, width, height};
    } catch(...) {
        throw std::runtime_error("[Rectangle::CreateFromTokens] Parsing error. The format must be four numbers: <left> <top> <width> <height>.");
    }
}
