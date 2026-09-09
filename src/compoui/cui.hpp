#pragma once

#include <memory>
#include "rect.hpp"
#include "vec2.hpp"

namespace CUI {
    class Element;

    class Context : public std::enable_shared_from_this<Context> {
    public:
        Vec2 windowSize;
        std::shared_ptr<Element> root;

        unsigned int vao;
        unsigned int vbo;
        
        Context(uint w, uint h, std::shared_ptr<Element> root);
        void resize(unsigned int w, unsigned int h);
        void draw();
    };

    typedef const std::shared_ptr<Context>& ContextPtr;

    void drawQuadAt(ContextPtr ctx, Rect rect);

    class Element {
    public:
        virtual void draw(ContextPtr ctx, CUI::Rect rect) {};
        virtual Vec2 naturalSize(Vec2 availableSpace) { return {0,0}; };
    };
}

