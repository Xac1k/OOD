#pragma once
#include <fstream>
#include <memory>

#include "../ICanvas.h"

namespace gfx {

class SVGCanvas : public ICanvas {
public:
    explicit SVGCanvas(const std::string& filename, unsigned int width, unsigned int height);
    ~SVGCanvas() override;

    void DrawEllipse(double cx, double cy, double rx, double ry) override;
    void DrawText(double left, double top, double fontSize, const std::string& text) override;
    void LineTo(double x, double y) override;
    void MoveTo(double x, double y) override;
    void SetColor(Color color) override;

private:
    std::ofstream m_outfile;
    double m_currentX = 0.0;
    double m_currentY = 0.0;
    unsigned int m_width;
    unsigned int m_height;
    Color m_color{};

    void WriteHeader();
    void WriteFooter();
};

} // gfx
