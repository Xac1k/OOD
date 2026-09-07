#include "Color.h"
#include <format>
#include <stdexcept>

Color::Color(const uint8_t red, const uint8_t green, const uint8_t blue)
: m_red(red), m_green(green), m_blue(blue)
{}

uint8_t Color::Blue() const {
    return m_blue;
}

uint8_t Color::Green() const {
    return m_green;
}

uint8_t Color::Red() const {
    return m_red;
}

Color Color::FromString(const std::string& str) {
    if (!str.starts_with('#')) {
        throw ColorException("[Color::FromString] String must starts with '#' and over all to be in format #RRGGBB.");
    };

    if (str.size() != 7) {
        throw ColorException("[Color::FromString] String must have 7 characters and over all to be in format #RRGGBB.");
    }

    uint8_t red, green, blue;
    try {
        red = std::stoi(str.substr(1, 2), nullptr, 16);
        green = std::stoi(str.substr(3, 2), nullptr, 16);
        blue = std::stoi(str.substr(5, 2), nullptr, 16);
    } catch (...) {
        throw ColorException("[Color::FromString] Invalid numbers value in string. The numbers must be in range of '0' to 'F' in hexadecimal system.");
    }

    return {red, green, blue};
}

std::string Color::ToString() {
    return "#"
    + std::format("{:02X}", m_red)
    + std::format("{:02X}", m_green)
    + std::format("{:02X}", m_blue);
}
