#pragma once
#include "../CRTPShapeGeometry.h"

namespace shapes
{

class Circle : public CRTPShapeGeometry<Circle> {
public:
    Circle(double x, double y, double radius);
    [[nodiscard]] std::unique_ptr<ShapeGeometry> Clone() const override;
    std::string GetTypeName() override;
    void Draw(gfx::ICanvas& canvas) override;
    std::vector<std::string> GetParams() override;
    void MoveShape(double dx, double dy) override;
    static Circle CreateFromTokens(const std::vector<std::string>& tokens);
private:
    double m_x = 0;
    double m_y = 0;
    double m_radius = 0;
};

}

