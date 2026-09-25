#pragma once
#include "../CRTPShapeGeometry.h"

namespace shapes {
class Rectangle : public CRTPShapeGeometry<Rectangle> {
public:
    Rectangle(double left, double top, double width, double height);
    [[nodiscard]] std::unique_ptr<ShapeGeometry> Clone() const override;
    std::string GetTypeName() override;
    void Draw(gfx::ICanvas& canvas) override;
    std::vector<std::string> GetParams() override;
    void MoveShape(double dx, double dy) override;
    static Rectangle CreateFromTokens(const std::vector<std::string>& tokens);
private:
    double m_left = 0, m_top = 0, m_width = 0, m_height = 0;
};
}

