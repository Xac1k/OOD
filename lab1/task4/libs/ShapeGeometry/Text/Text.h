#pragma once
#include "../CRTPShapeGeometry.h"

namespace shapes {
class Text : public CRTPShapeGeometry<Text> {
public:
    Text(double left, double top, int fontSize, std::string  text);
    [[nodiscard]] std::unique_ptr<ShapeGeometry> Clone() const override;
    std::string GetTypeName() override;
    void Draw(gfx::ICanvas& canvas) override;
    std::vector<std::string> GetParams() override;
    void MoveShape(double dx, double dy) override;
    static Text CreateFromTokens(const std::vector<std::string>& tokens);
private:
    double m_left = 0;
    double m_top = 0;
    int m_fontSize = 0;
    std::string m_text;
};
}

