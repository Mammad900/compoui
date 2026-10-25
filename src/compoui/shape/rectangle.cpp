#include "rectangle.hpp"

const char* CUI::Shape::Rectangle::source = R"(
    float sdf() {
        vec2 halfSize = vLocalPixel / vLocalUV * 0.5;
        vec2 d = abs(vLocalPixel - halfSize) - halfSize;
        return max(d.x, d.y);
    }
)";

void CUI::Shape::Rectangle::setUniforms() {

}