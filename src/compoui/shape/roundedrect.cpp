#include "roundedrect.hpp"
#include "../glad/glad.h"

const char* CUI::Shape::RoundedRect::source = R"(
    layout(location = 10) uniform float uRadius;
    float sdf() {
        vec2 halfSize = vLocalPixel / vLocalUV * 0.5;
        vec2 dv = abs(vLocalPixel - halfSize) - halfSize + uRadius;
        float d = (dv.x > 0.0 && dv.y > 0.0) ? length(dv) : max(dv.x, dv.y);
        return d - uRadius;
    }
)";

void CUI::Shape::RoundedRect::setUniforms() {
    glUniform1f(10, radius.get());
}