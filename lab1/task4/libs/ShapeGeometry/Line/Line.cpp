#include "Line.h"

shapes::Line::Line(const double x1, const double y1, const double x2, const double y2)
: m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
{}

std::vector<std::string> shapes::Line::GetParams()
{
    return {
        std::to_string(m_x1),
        std::to_string(m_y1),
        std::to_string(m_x2),
        std::to_string(m_y2)
    };
}

std::string shapes::Line::GetTypeName() {
    return "line";
}

void shapes::Line::MoveShape(double dx, double dy) {
    m_x1 += dx;
    m_y1 += dy;
    m_x2 += dx;
    m_y2 += dy;
}

std::unique_ptr<shapes::ShapeGeometry> shapes::Line::Clone() const {
    Line newLine(m_x1, m_y1, m_x2, m_y2);

    return std::make_unique<Line>(newLine);
}

void shapes::Line::Draw(gfx::ICanvas& canvas) {
    canvas.MoveTo(m_x1, m_y1);
    canvas.LineTo(m_x2, m_y2);
}

shapes::Line shapes::Line::CreateFromTokens(const std::vector<std::string>& tokens) {
    try {
        const auto x1 = std::stod(tokens.at(0));
        const auto y1 = std::stod(tokens.at(1));
        const auto x2 = std::stod(tokens.at(2));
        const auto y2 = std::stod(tokens.at(3));
        return {x1, y1, x2, y2};
    } catch(...) {
        throw std::runtime_error("[Line::CreateFromTokens] Parsing error. The format must be four numbers: <x1> <y1> <x2> <y2>.");
    }
}