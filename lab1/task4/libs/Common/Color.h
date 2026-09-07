#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>

class ColorException : public std::invalid_argument {
public:
    explicit ColorException(const std::string& message) : std::invalid_argument(message) {}
};

class Color {
public:
    Color() = default;
    Color(uint8_t red, uint8_t green, uint8_t blue);
    std::string ToString();
    static Color FromString(const std::string& str);
    [[nodiscard]] uint8_t Red() const;
    [[nodiscard]] uint8_t Green() const;
    [[nodiscard]] uint8_t Blue() const;
private:
    uint8_t m_red = 0;
    uint8_t m_green = 0;
    uint8_t m_blue = 0;
};
