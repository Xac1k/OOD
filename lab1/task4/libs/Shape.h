#pragma once
#include <memory>
#include "Common/Color.h"
#include "ShapeGeometry/CRTPShapeGeometry.h"

namespace shapes {

class Shape {
public:
    Shape(std::unique_ptr<ShapeGeometry> geometry, Color c);
    [[nodiscard]] std::string GetTypeName() const;
    void SetColor(Color newColor);
    void MoveShape(double dx, double dy) const;
    void SetGeometry(std::unique_ptr<ShapeGeometry>);
    [[nodiscard]] std::vector<std::string> GetParams() const;
    [[nodiscard]] std::unique_ptr<Shape> Clone() const;
    void Draw(gfx::ICanvas&) const;
    [[nodiscard]] Color GetColor() const;

private:
    Color m_color;
    std::unique_ptr<ShapeGeometry> m_geometry;
};

} // shapes
