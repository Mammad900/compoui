#pragma once

#include "primitive.hpp"
#include <memory>
#include <unordered_map>
#include "../../shape/shape.hpp"

namespace CUI::Primitive {

    class ShapePrimitive : public Primitive {
    public:
        struct ShaderKey {
            const char* first;   // e.g. sdf source pointer
            const char* second;  // e.g. fragment shader pointer

            bool operator==(ShaderKey const& other) const noexcept {
                return first == other.first && second == other.second;
            }

        };
        struct ShaderKeyHasher {
            static std::size_t operator()(ShaderKey const& p) noexcept;
        };
        std::shared_ptr<Shape::Shape> shape;

        // this constructor needs to make a ShaderKey and get cached shader program
        // if not, make one:
        // get SDF source, concatenate with frag source, call Primitive's constructor and store in cache
        ShapePrimitive(std::shared_ptr<Shape::Shape> shape, const char* fragmentSource);

        void setDerivedUniforms(CUI::Rect) override {
            shape->setUniforms();
        }
    };
}