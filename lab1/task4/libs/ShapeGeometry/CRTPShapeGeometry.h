#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../Canvas/ICanvas.h"
#include <concepts>

namespace shapes {

    template<typename T>
    concept CreateableFromTokens = requires(const std::vector<std::string>& tokens) {
        { T::CreateFromTokens(tokens) } -> std::same_as<T>;
    };

    class ShapeGeometry {
    public:
        virtual ~ShapeGeometry() = default;
        virtual std::string GetTypeName() = 0;
        virtual std::vector<std::string> GetParams() = 0;
        virtual void MoveShape(double dx, double dy) = 0;
        virtual void Draw(gfx::ICanvas& canvas) = 0;
        [[nodiscard]] virtual std::unique_ptr<ShapeGeometry> Clone() const = 0;
    };

    template <typename Derived>
    class CRTPShapeGeometry : public ShapeGeometry {
    public:
        CRTPShapeGeometry() {
            static_assert(CreateableFromTokens<Derived>,
                "Derived class must implement static Derived CreateFromTokens(const std::vector<std::string>& tokens)");
        }
    };

} // shapes

