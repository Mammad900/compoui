#pragma once

#include <memory>
#include "rect.hpp"
#include "vec2.hpp"
#include <vector>
#include <optional>

namespace CUI {
    class Element;
    class HitBoxEl;
    
    struct MouseEvent {
        enum Type {
            down, up, move, enter, leave, hover, click
        };
        struct Buttons {
            bool left:1, right:1, middle:1, extra1:1, extra2:1;
        };
        enum Button {
            left, right, middle, extra1, extra2
        };
        Type type;
        Buttons buttons;
        std::optional<Button> button;
        std::optional<Vec2> position; // mouse position relative to the window
        std::optional<Vec2> relativePosition; // only for processed events, mouse position relative to the HitBox rect
        std::shared_ptr<HitBoxEl> hitBox;
    };

    class Context : public std::enable_shared_from_this<Context> {
    public:
        Vec2 windowSize;
        std::shared_ptr<Element> root;

        unsigned int vao;
        unsigned int vbo;

        std::vector<std::shared_ptr<HitBoxEl>> hitBoxes;
        std::optional<Vec2> mousePosition;
        MouseEvent::Buttons mouseButtons;
        std::shared_ptr<Element> capture;
        std::vector<MouseEvent> pendingEvents;
        
        Context(uint w, uint h, std::shared_ptr<Element> root);
        void resize(unsigned int w, unsigned int h);
        void draw();

        void mouseEvent(MouseEvent event);
        void postEvents();
    };

    typedef const std::shared_ptr<Context>& ContextPtr;

    void drawQuadAt(ContextPtr ctx, Rect rect);

    class Element {
    public:
        virtual void draw(ContextPtr ctx, CUI::Rect rect) {};
        virtual Vec2 naturalSize(Vec2 availableSpace) { return {0,0}; };
    };
}

