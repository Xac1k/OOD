#pragma once
#include "../CRTPShapeGeometry.h"

namespace shapes {
class Triangle : public CRTPShapeGeometry<Triangle> {
public:
    [[nodiscard]] std::unique_ptr<ShapeGeometry> Clone() const override;
    std::string GetTypeName() override;
    void Draw(gfx::ICanvas& canvas) override;
    std::vector<std::string> GetParams() override;
    void MoveShape(double dx, double dy) override;
    Triangle(double x1, double y1, double x2, double y2, double x3, double y3);
    static Triangle CreateFromTokens(const std::vector<std::string>& tokens);

private:
    double m_x1;
    double m_y1;
    double m_x2;
    double m_y2;
    double m_x3;
    double m_y3;
};
}
