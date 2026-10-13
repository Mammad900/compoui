#pragma once

#include <memory>
#include "rect.hpp"
#include "vec2.hpp"
#include "mouse-event.hpp"
#include <vector>
#include <optional>
#include <chrono>

namespace CUI {
    class Element;
    class HitBoxEl;
    class PendingLayer;
    class FBO;
    class FBOLease;
    namespace Misc {
        class Layer;
    }
    
    // Contains info about a GUI window.
    class Context : public std::enable_shared_from_this<Context> {
    public:
        // Window size in pixels. Do not set, use resize().
        Vec2 windowSize;

        // The root element. This element is rendered with the rect filling the window.
        std::shared_ptr<Misc::Layer> root;

        // The current clip boundary. Is set while drawing a Clip element and its children.
        std::optional<Rect> clipRect;

        PendingLayer *currentLayer = nullptr;

        std::shared_ptr<FBO> currentFBO;
        std::shared_ptr<FBO> mainFBO;
        
        std::shared_ptr<FBOLease> getFBO();

        // OpenGL VAO used to render a quad. Consider readonly.
        unsigned int vao;
        // OpenGL VBO used to render a quad. Consider readonly.
        unsigned int vbo;

        // The current mouse position. nullopt if outside the window.
        std::optional<Vec2> mousePosition;
        // The current state of mouse buttons.
        MouseEvent::Buttons mouseButtons;
        // If LMB is pressed on a consuming hit-box, this is set to it, until its released. Otherwise nullptr.
        std::shared_ptr<Element> capture;
        
        // Create GUI context with given window size and root element.
        Context(uint w, uint h, std::shared_ptr<Misc::Layer> root);
        Context(uint w, uint h, std::shared_ptr<Element> root);
        // Resize window
        void resize(unsigned int w, unsigned int h);
        // Draw the GUI.
        void draw();
        void display();

        void pasteFBO(std::shared_ptr<FBO> &fbo);
        void activateFBO(std::shared_ptr<FBO> &fbo, bool clear = false);

        // Send a raw mouse event to be processed
        void mouseEvent(MouseEvent event);
        // Call this after all mouse event for this frame are sent.
        void postEvents();

        // Time between this frame and the last, in seconds
        float deltaTime;

        // Sets the current clip boundary. Used by the Clip element
        void setClip(std::optional<Rect> rect);
    private:
        std::vector<std::shared_ptr<HitBoxEl>> hitBoxes;
        std::vector<MouseEvent> pendingEvents;
        std::vector<std::shared_ptr<FBO>> fboPool;
        friend class HitBoxEl;
        std::chrono::steady_clock::time_point lastFrame;
    };

    typedef const std::shared_ptr<Context>& ContextPtr;

    // The smallest building block of a UI.
    class Element {
    public:
        // Draw this element at the given position and size
        virtual void draw(ContextPtr ctx, CUI::Rect rect) {};

        // Get the natural size of this element, given the available space. Used by some layout elements.
        virtual Vec2 naturalSize(Vec2 availableSpace) { return {0,0}; };
    };

    class PendingLayer {
    public:
        Rect rect;
        std::shared_ptr<Element> root;
        std::vector<PendingLayer> children;

        void draw(ContextPtr ctx);
    };
}

