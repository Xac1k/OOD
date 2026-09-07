#include "DrawerIO.h"
#include <chrono>
#include <iostream>
#include "Canvas/SVGCanvas/SVGCanvas.h"
#include "ShapeGeometry/Circle/Circle.h"
#include "ShapeGeometry/Line/Line.h"
#include "ShapeGeometry/Rectangle/Rectangle.h"
#include "ShapeGeometry/Text/Text.h"
#include "ShapeGeometry/Triangle/Triangle.h"

namespace {
std::string GenerateFileName() {
    return std::format(
        "Файл-изображения-{:%Y-%m-%d %H:%M:%S}.svg",
        std::chrono::system_clock::now()
    );
}

void PrintErrorSignature(const std::string& signature) {
    std::cout <<  "\033[101m" << "\nWrong number of arguments.\n"
                << "The signature is '" << signature << "'\n"
                << "Please enter missing arguments.\n" << "\033[0m" << std::endl;
}
}

DrawerIO::DrawerIO() {
    shapes::Picture picture(std::make_unique<gfx::SVGCanvas>(GenerateFileName(), 1920, 1080));

    m_picture = std::make_unique<shapes::Picture>(std::move(picture));
}

void DrawerIO::HandleInput(const std::vector<std::string>& tokens) const {
    if (tokens.empty()) {
        return;
    }

    const auto& command = tokens[0];

    try {
        if (command == "AddShape") {
            if (tokens.size() < 4) {
                PrintErrorSignature("AddShape <id> <цвет> <тип> <параметры>");
            }

            const auto& id = tokens[1];
            const auto color = Color::FromString(tokens[2]);
            const auto& type = tokens[3];
            const std::vector remainedTokens(tokens.begin() + 4, tokens.end());

            auto geometry = CreateShapeGeometryByType(type, remainedTokens);
            auto shape = std::make_unique<shapes::Shape>(std::move(geometry), color);

            m_picture->AddShape(id, std::move(shape));
        }
        else if (command == "MoveShape") {
            if (tokens.size() != 4) {
                PrintErrorSignature("MoveShape <id> <dx> <dy>");
            }
            const auto dx = std::stoi(tokens[2]);
            const auto dy = std::stoi(tokens[3]);

            m_picture->MoveShape(tokens[1], dx, dy);
        }
        else if (command == "MovePicture") {
            if (tokens.size() != 3) {
                PrintErrorSignature("MovePicture <dx> <dy>");
            }
            const auto dx = std::stoi(tokens[1]);
            const auto dy = std::stoi(tokens[2]);

            m_picture->MovePicture(dx, dy);
        }
        else if (command == "DeleteShape") {
            if (tokens.size() != 2) {
                PrintErrorSignature("DeleteShape <id>");
            }
            m_picture->DeleteShape(tokens[1]);
        }
        else if (command == "List") {
            std::cout << m_picture->List() << std::endl;
        }
        else if (command == "ChangeColor") {
            if (tokens.size() != 3) {
                PrintErrorSignature("ChangeColor <id> <цвет>");
            }

            const auto color = Color::FromString(tokens[2]);
            m_picture->ChangeColor(tokens[1], color);
        }
        else if (command == "ChangeShape") {
            if (tokens.size() < 3) {
                PrintErrorSignature("ChangeShape <id> <тип> <параметры>");
            }

            const auto& id = tokens[1];
            const auto& type = tokens[2];
            const std::vector remainedTokens(tokens.begin() + 3, tokens.end());

            auto geometry = CreateShapeGeometryByType(type, remainedTokens);

            m_picture->ChangeShape(id, std::move(geometry));
        }
        else if (command == "DrawShape") {
            if (tokens.size() != 2) {
                PrintErrorSignature("DrawShape <id>");
            }
            m_picture->DrawShape(tokens[1]);
        }
        else if (command == "DrawPicture") {
            m_picture->DrawPicture();
        }
    } catch (std::exception& e) {
        std::cout << "\033[97m\033[41m" << e.what() << "\033[0m" << std::endl;
    }
}

std::unique_ptr<shapes::ShapeGeometry> DrawerIO::CreateShapeGeometryByType(
    const std::string& type,
    const std::vector<std::string>& tokens
) {
    if (type == "circle") {
        auto circle = shapes::Circle::CreateFromTokens(tokens);
        return std::make_unique<shapes::Circle>(std::move(circle));
    }
    if (type == "rectangle") {
        auto rectangle = shapes::Rectangle::CreateFromTokens(tokens);
        return std::make_unique<shapes::Rectangle>(std::move(rectangle));
    }
    if (type == "triangle") {
        auto triangle = shapes::Triangle::CreateFromTokens(tokens);
        return std::make_unique<shapes::Triangle>(std::move(triangle));
    }
    if (type == "line") {
        auto line = shapes::Line::CreateFromTokens(tokens);
        return std::make_unique<shapes::Line>(std::move(line));
    }
    if (type == "text") {
        auto text = shapes::Text::CreateFromTokens(tokens);
        return std::make_unique<shapes::Text>(std::move(text));
    }
    throw std::runtime_error("Unknown shape type: " + type);
}
