#pragma once
#include <map>
#include <memory>
#include <string>
#include "Shape.h"
#include "Canvas/ICanvas.h"
#include "ShapeGeometry/CRTPShapeGeometry.h"

namespace shapes
{
    class Picture;

    class ShapeExistenceException : public std::runtime_error {
public:
    explicit ShapeExistenceException(const std::string& message) : std::runtime_error(message) {}
};

class ShapeObserver : public Observer<Shape> {
public:
    explicit ShapeObserver(Picture& picture);
    void Update(const Shape& data) override;

private:
    Picture& m_picture;
};
enum class PictureEvenType { ShapeChanged, ShapeAdded, ShapeRemoved };

class Picture : public Observable<Picture, PictureEvenType> {
public:
    using EventType = PictureEvenType;

    explicit Picture(std::unique_ptr<gfx::ICanvas> canvas);
    void DrawPicture() noexcept;
    void DrawShape(const std::string& ID);
    void AddShape(const std::string& ID, std::unique_ptr<Shape> shape);
    [[nodiscard]] Shape& GetShape(const std::string& ID) const;
    void MoveShape(const std::string& ID, double dx, double dy);
    void MovePicture(double dx, double dy) noexcept;
    void DeleteShape(const std::string& ID);
    [[nodiscard]] std::string List() const noexcept;
    [[nodiscard]] size_t GetShapesCount() const noexcept;
    void ChangeColor(const std::string& ID, Color newColor);
    void ChangeShape(const std::string& ID, std::unique_ptr<ShapeGeometry> newGeometry);
    void CloneShape(const std::string& ID, const std::string& newID);
    Picture Clone(std::unique_ptr<gfx::ICanvas> canvas);
private:
    std::map<std::string, std::unique_ptr<Shape>> m_doodles;
    std::unique_ptr<gfx::ICanvas> m_canvas;
    ShapeObserver m_shapeObserver;
};

} // shapes

