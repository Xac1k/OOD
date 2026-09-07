#pragma once
#include "../CRTPShapeGeometry.h"

namespace shapes {

class Line : public CRTPShapeGeometry<Line> {
public:
    Line(double x1, double y1, double x2, double y2);
    [[nodiscard]] std::unique_ptr<ShapeGeometry> Clone() const override;
    std::string GetTypeName() override;
    void Draw(gfx::ICanvas& canvas) override;
    std::vector<std::string> GetParams() override;
    void MoveShape(double dx, double dy) override;
    static Line CreateFromTokens(const std::vector<std::string>& tokens);
private:
    double m_x1 = 0;
    double m_y1 = 0;
    double m_x2 = 0;
    double m_y2 = 0;
};

}
