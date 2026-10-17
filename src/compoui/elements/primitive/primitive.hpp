#pragma once

#include "../../cui.hpp"

namespace CUI::Primitive {
    // The base class providing most functionality needed to draw quad based elements
    class Primitive : public Element {
    public:
        Primitive(unsigned int &program, const char *fragmentSource);
        void draw(ContextPtr &ctx, CUI::Rect rect);

    protected:
        unsigned int &program;
        /* Derived classes override this if they need to push extra uniforms. */
        virtual void setDerivedUniforms(ContextPtr, CUI::Rect) = 0;
    };
}
