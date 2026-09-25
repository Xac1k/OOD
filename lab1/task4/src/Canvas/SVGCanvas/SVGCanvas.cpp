#include "SVGCanvas.h"
#include <filesystem>

gfx::SVGCanvas::SVGCanvas(const std::string& filename, unsigned int width, unsigned int height)
: m_width(width), m_height(height)
{
    const std::filesystem::path gpath(filename);
    m_outfile.open(gpath);
    if (!m_outfile.is_open()) {
        throw std::runtime_error("[SVGCanvas] Can't open svg file: " + gpath.string());
    }

    WriteHeader();
}

void gfx::SVGCanvas::WriteHeader() {
    m_outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    m_outfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" \n";
    m_outfile << "     width=\"" << m_width << "\" height=\"" << m_height << "\" \n";
    m_outfile << "     viewBox=\"0 0 " << m_width << " " << m_height << "\">\n";
}

gfx::SVGCanvas::~SVGCanvas() {
    WriteFooter();
    m_outfile.close();
}

void gfx::SVGCanvas::WriteFooter() {
    m_outfile << "</svg>\n";
}

void gfx::SVGCanvas::SetColor(const Color color) {
    m_color = color;
}

void gfx::SVGCanvas::MoveTo(const double x, const double y) {
    m_currentX = x;
    m_currentY = y;
}

void gfx::SVGCanvas::LineTo(const double x, const double y) {
     m_outfile << "  <line x1=\"" << m_currentX << "\" y1=\"" << m_currentY
        << "\" x2=\"" << x << "\" y2=\"" << y
        << "\" stroke=\"" << m_color.ToString() << "\" "
        << "stroke-width=\"2\" />\n";
    m_currentX = x;
    m_currentY = y;
}

void gfx::SVGCanvas::DrawEllipse(const double cx, const double cy, const double rx, const double ry) {
    m_outfile << "  <ellipse cx=\"" << cx << "\" cy=\"" << cy
            << "\" rx=\"" << rx << "\" ry=\"" << ry
            << "\" fill=\"" << m_color.ToString() << "\" "
            << "fill-opacity=\"1.0\" />\n";
}

void gfx::SVGCanvas::DrawText(const double left, const double top, const double fontSize, const std::string& text) {
    m_outfile << "  <text x=\"" << left << "\" y=\"" << top
            << "\" font-size=\"" << fontSize
            << "\" fill=\"" << m_color.ToString() << "\" "
            << "font-family=\"Arial, sans-serif\">"
            << text
            << "</text>\n";
}

