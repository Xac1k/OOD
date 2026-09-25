#include "Shape.h"

Color shapes::Shape::GetColor() const{
    return m_color;
}

void shapes::Shape::Draw(gfx::ICanvas& canvas) const {
    canvas.SetColor(m_color);

    m_geometry->Draw(canvas);
}

std::string shapes::Shape::GetTypeName() const {
    return m_geometry->GetTypeName();
}

void shapes::Shape::SetColor(const Color newColor) {
    m_color = newColor;
    Notify(EventType::ColorChanged);
}

void shapes::Shape::MoveShape(const double dx, const double dy) const {
    m_geometry->MoveShape(dx, dy);
    Notify(EventType::PositionChanged);
}

void shapes::Shape::SetGeometry(std::unique_ptr<ShapeGeometry> newGeometry) {
    m_geometry.swap(newGeometry);
    Notify(EventType::GeometryChanged);
}

std::vector<std::string> shapes::Shape::GetParams() const {
    return m_geometry->GetParams();
}

std::unique_ptr<shapes::Shape> shapes::Shape::Clone() const {
    auto clone = std::make_unique<Shape>(m_geometry->Clone(), m_color);
    return clone;
}

shapes::Shape::Shape(std::unique_ptr<ShapeGeometry> geometry, const Color c)
: m_color(c)
, m_geometry(std::move(geometry))
{
    if (!m_geometry) {
        throw std::invalid_argument("m_geometry cannot be null");
    }
}
