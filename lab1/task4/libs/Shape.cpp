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
}

void shapes::Shape::MoveShape(const double dx, const double dy) const {
    m_geometry->MoveShape(dx, dy);
}

void shapes::Shape::SetGeometry(std::unique_ptr<ShapeGeometry>) {
    m_geometry.swap(m_geometry);
}

std::vector<std::string> shapes::Shape::GetParams() const {
    return m_geometry->GetParams();
}

std::unique_ptr<shapes::Shape> shapes::Shape::Clone() const {
    std::unique_ptr<Shape> clone;
    clone->SetColor(m_color);
    clone->SetGeometry(m_geometry->Clone());

    return clone;
}

shapes::Shape::Shape(std::unique_ptr<ShapeGeometry> geometry, const Color c)
: m_color(c), m_geometry(std::move(geometry))
{}
