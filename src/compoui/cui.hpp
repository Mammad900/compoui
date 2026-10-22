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
    using ElementPtr = std::shared_ptr<Element>;

    enum Orientation {vertical, horizontal};
    
    // Contains info about a GUI window.
    class Context {
    public:
        // Window size in pixels. Do not set, use resize().
        Vec2 windowSize;

        // The root element. This element is rendered with the rect filling the window.
        std::shared_ptr<Misc::Layer> root;

        // [advanced] The current clip boundary. Is set while drawing a Clip element and its children.
        std::optional<Rect> clipRect;

        // [advanced] The innermost layer currently being rendered, may be null.
        PendingLayer *currentLayer = nullptr;

        // [advanced] The currently bound framebuffer
        std::shared_ptr<FBO> currentFBO;

        // The framebuffer that contains the main UI.
        std::shared_ptr<FBO> mainFBO;
        
        // Lease a temporary framebuffer from a pool, creating one if necessary.
        std::shared_ptr<FBOLease> getFBO();

        // [advanced] OpenGL VAO used to render a quad. Consider readonly.
        unsigned int vao;
        // [advanced] OpenGL VBO used to render a quad. Consider readonly.
        unsigned int vbo;

        // The current mouse position. nullopt if outside the window.
        std::optional<Vec2> mousePosition;
        // The current state of mouse buttons.
        MouseEvent::Buttons mouseButtons;
        // If LMB is pressed on a consuming hit-box, this is set to it, until its released. Otherwise nullptr.
        ElementPtr capture;
        
        // Create GUI context with given window size and root element.
        Context(uint w, uint h, std::shared_ptr<Misc::Layer> root);
        Context(uint w, uint h, ElementPtr root);

        // Resize window. Must not be called during draw()
        void resize(unsigned int w, unsigned int h);

        // Draw the GUI to the main framebuffer. Make sure to call display() afterwards or present it yourself.
        void draw();

        // Present the main framebuffer to the window. Alternatively, present it yourself by using `mainFBO`.
        void display();

        // [advanced] Paste the contents of the given framebuffer to the currently bound framebuffer
        void pasteFBO(std::shared_ptr<FBO> &fbo);

        // [advanced] Activate a framebuffer and optionally clear it
        void activateFBO(std::shared_ptr<FBO> &fbo, bool clear = false);

        // Send a raw mouse event to be processed
        void mouseEvent(MouseEvent event);
        // Call this after all mouse event for this frame are sent.
        void postEvents();

        // Time between this frame and the last, in seconds
        float deltaTime;

        // [advanced] Sets the current clip boundary. Used by the Clip element
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
        virtual void draw(CUI::Rect rect) {};

        // Get the natural size of this element, given the available space. Used by some layout elements.
        virtual Vec2 naturalSize(Vec2 availableSpace) { return {0,0}; };
    };

    class PendingLayer {
    public:
        Rect rect;
        ElementPtr root;
        std::vector<PendingLayer> children;

        void draw();
    };

    // The currently active CUI context. 
    // Do not forget to set this when interacting with the context!
    // Is thread local.
    extern thread_local Context* ctx;
}

