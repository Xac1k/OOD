#include "Picture.h"

#include <ostream>
#include <ranges>

shapes::Picture::Picture(std::unique_ptr<gfx::ICanvas> canvas)
: m_canvas(std::move(canvas))
{}

void shapes::Picture::AddShape(const std::string& ID, std::unique_ptr<Shape> shape) {
    const auto doodleIter = m_doodles.find(ID);
    if (doodleIter != m_doodles.end()) {
        throw ShapeExistenceException("[AddShape] Shape with ID: " + ID + " already exists.");
    }

    m_doodles[ID] = std::move(shape);
}

void shapes::Picture::ChangeColor(const std::string& ID, const Color newColor) {
    const auto doodleIter = m_doodles.find(ID);
    if (doodleIter == m_doodles.end()) {
        throw ShapeExistenceException("[ChangeColor] Shape with ID: " + ID + " don't exists.");
    }

    doodleIter->second->SetColor(newColor);
}

void shapes::Picture::DrawPicture() noexcept {
    for (const auto& shape : m_doodles | std::views::values) {
        shape->Draw(*m_canvas);
    }
}

std::string shapes::Picture::List() const noexcept {
    std::string result;

    size_t shapeNumber = 1;
    for (const auto& [id, shape] : m_doodles) {
        result += std::to_string(shapeNumber) + " " + shape->GetTypeName() + " " + id + " " +
            shape->GetColor().ToString();

        auto params = shape->GetParams();
        for (const auto& param : params) {
            result += " " + param;
        }

        result += "\n";

        shapeNumber++;
    }

    return result;
}

void shapes::Picture::DrawShape(const std::string& ID) {
    const auto doodleIter = m_doodles.find(ID);
    if (doodleIter == m_doodles.end()) {
        throw ShapeExistenceException("[DrawShape] Shape with ID: " + ID + " don't exists.");
    }

    doodleIter->second->Draw(*m_canvas);
}

void shapes::Picture::DeleteShape(const std::string& ID) {
    const auto doodleIter = m_doodles.find(ID);
    if (doodleIter == m_doodles.end()) {
        throw ShapeExistenceException("[DeleteShape] Shape with ID: " + ID + " don't exists.");
    }

    m_doodles.erase(doodleIter);
}

void shapes::Picture::MovePicture(const double dx, const double dy) noexcept {
    for (auto& shape : m_doodles | std::views::values) {
        shape->MoveShape(dx, dy);
    }
}

void shapes::Picture::MoveShape(const std::string& ID, const double dx, const double dy) {
    const auto doodleIter = m_doodles.find(ID);
    if (doodleIter == m_doodles.end()) {
        throw ShapeExistenceException("[MoveShape] Shape with ID: " + ID + " don't exists.");
    }

    doodleIter->second->MoveShape(dx, dy);
}

void shapes::Picture::ChangeShape(const std::string& ID, std::unique_ptr<ShapeGeometry> newGeometry) {
    const auto doodleIter = m_doodles.find(ID);
    if (doodleIter == m_doodles.end()) {
        throw ShapeExistenceException("[ChangeShape] Shape with ID: " + ID + " don't exists.");
    }

    doodleIter->second->SetGeometry(std::move(newGeometry));
}

void shapes::Picture::CloneShape(const std::string& ID, const std::string& newID) {
    const auto doodleIter = m_doodles.find(ID);
    if (doodleIter == m_doodles.end()) {
        throw ShapeExistenceException("[CloneShape] Shape with ID: " + ID + " don't exists.");
    }

    AddShape(newID, doodleIter->second->Clone());
}
