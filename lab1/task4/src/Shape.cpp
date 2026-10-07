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
    m_onColorChanged.Notify(newColor);
}

void shapes::Shape::MoveShape(const double dx, const double dy) {
    m_geometry->MoveShape(dx, dy);
    m_onPositionChanged.Notify(dx, dy);
}

void shapes::Shape::SetGeometry(std::unique_ptr<ShapeGeometry> newGeometry) {
    m_geometry.swap(newGeometry);
    m_onShapeGeometryChanged.Notify(newGeometry.get());
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
