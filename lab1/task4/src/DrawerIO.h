#pragma once
#include <iostream>

#include "Picture.h"

class PictureObserver : public Observer<shapes::Picture> {
public:
    explicit PictureObserver(std::ostream& os): m_os(os) {};

    void Update(const shapes::Picture& data) override {
        m_os << "Picture was changed. Shapes count:" << data.GetShapesCount() << std::endl;
    };

private:
    std::ostream& m_os;
};

class DrawerIO {
public:
    DrawerIO();
    ~DrawerIO();
    void HandleInput(const std::vector<std::string>& tokens) const;
private:
    static std::unique_ptr<shapes::ShapeGeometry> CreateShapeGeometryByType(
        const std::string& type,
        const std::vector<std::string>& tokens
    );
    PictureObserver m_pictureObserver;
    std::unique_ptr<shapes::Picture> m_picture;
};
