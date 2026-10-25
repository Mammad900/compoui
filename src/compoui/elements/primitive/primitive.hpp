#pragma once

#include "../../cui.hpp"
#include <functional>

namespace CUI::Primitive {
    // The base class providing most functionality needed to draw quad based elements
    class Primitive : public Element {
    public:
        Primitive(unsigned int &program, const char *fragmentSource);
        Primitive(unsigned int &program, std::function<std::string()>);
        void draw(Rect rect);

    protected:
        unsigned int &program;
        /* Derived classes override this if they need to push extra uniforms. */
        virtual void setDerivedUniforms(CUI::Rect) = 0;
    };
}
