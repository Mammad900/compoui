#include "shape.hpp"
#include <unordered_map>
namespace CUI::Primitive {

namespace {
    static std::unordered_map<ShapePrimitive::ShaderKey, unsigned int, ShapePrimitive::ShaderKeyHasher> programs;
}

ShapePrimitive::ShapePrimitive(std::shared_ptr<Shape::Shape> shape, const char* fragmentSource) : Primitive(
    programs[ShaderKey(shape->getSdfSource(), fragmentSource)], 
    [&]() {return std::string(fragmentSource)+'\n'+shape->getSdfSource();}
), shape(shape) {}

std::size_t ShapePrimitive::ShaderKeyHasher::operator()(ShaderKey const& p) noexcept {
    // Combine the two pointer hashes (std::hash<const char*>() is defined).
    std::size_t h1 = std::hash<const char*>()(p.first);
    std::size_t h2 = std::hash<const char*>()(p.second);

    // A simple xor‑shift combine.  Many other mixes are fine.
    return h1 ^ (h2 << 1);
}

}