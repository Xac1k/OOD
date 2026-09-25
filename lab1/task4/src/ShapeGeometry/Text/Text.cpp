#include "Text.h"

#include <utility>

std::vector<std::string> shapes::Text::GetParams() {
    return {
        std::to_string(m_left),
        std::to_string(m_top),
        std::to_string(m_fontSize),
        m_text
    };
}

std::string shapes::Text::GetTypeName() {
    return "text";
}

void shapes::Text::MoveShape(const double dx, const double dy) {
    m_left += dx;
    m_top += dy;
}

void shapes::Text::Draw(gfx::ICanvas& canvas) {
    canvas.DrawText(m_left, m_top, m_fontSize, m_text);
}

std::unique_ptr<shapes::ShapeGeometry> shapes::Text::Clone() const {
    Text newText(m_left, m_top, m_fontSize, m_text);
    return std::make_unique<Text>(newText);
}

shapes::Text::Text(const double left, const double top, const int fontSize, std::string text)
: m_left(left), m_top(top), m_fontSize(fontSize), m_text(std::move(text))
{}

shapes::Text shapes::Text::CreateFromTokens(const std::vector<std::string>& tokens) {
    try {
        const auto left = std::stod(tokens.at(0));
        const auto top = std::stod(tokens.at(1));
        const auto fontSize = std::stoi(tokens.at(2));
        return {left, top, fontSize, tokens.at(3)};
    } catch(...) {
        throw std::runtime_error("[Text::CreateFromTokens] Parsing error. The format must be three numbers and string: <left> <top> <размер> <тест>.");
    }
}

