#pragma once
#include <map>
#include <memory>
#include <string>
#include "Shape.h"
#include "Canvas/ICanvas.h"
#include "ShapeGeometry/CRTPShapeGeometry.h"

namespace shapes
{

class ShapeExistenceException : public std::runtime_error {
public:
    explicit ShapeExistenceException(const std::string& message) : std::runtime_error(message) {}
};

class Picture {
public:
    explicit Picture(std::unique_ptr<gfx::ICanvas> canvas);
    void DrawPicture() noexcept;
    void DrawShape(const std::string& ID);
    void AddShape(const std::string& ID, std::unique_ptr<Shape> shape);
    void MoveShape(const std::string& ID, double dx, double dy);
    void MovePicture(double dx, double dy) noexcept;
    void DeleteShape(const std::string& ID);
    [[nodiscard]] std::string List() const noexcept;
    void ChangeColor(const std::string& ID, Color newColor);
    void ChangeShape(const std::string& ID, std::unique_ptr<ShapeGeometry> newGeometry);
    void CloneShape(const std::string& ID, const std::string& newID);
private:
    std::map<std::string, std::unique_ptr<Shape>> m_doodles;
    std::unique_ptr<gfx::ICanvas> m_canvas;
};

} // shapes

