#pragma once
#include <memory>
#include "Common/Color.h"
#include "Common/Event.h"
#include "ShapeGeometry/CRTPShapeGeometry.h"

namespace shapes {

class Shape {
    DECLARE_EVENT(ColorChanged, Color color)
    DECLARE_EVENT(ShapeGeometryChanged, ShapeGeometry* shapeGeometry)
    DECLARE_EVENT(PositionChanged, double dx, double dy)
    
public:
    Shape(std::unique_ptr<ShapeGeometry> geometry, Color c);
    [[nodiscard]] std::string GetTypeName() const;
    void SetColor(Color newColor);
    void MoveShape(double dx, double dy);
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
