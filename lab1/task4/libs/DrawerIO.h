#pragma once
#include "Picture.h"

class DrawerIO {
public:
    DrawerIO();
    void HandleInput(const std::vector<std::string>& tokens) const;
private:
    static std::unique_ptr<shapes::ShapeGeometry> CreateShapeGeometryByType(
        const std::string& type,
        const std::vector<std::string>& tokens
    );
    std::unique_ptr<shapes::Picture> m_picture;
};
