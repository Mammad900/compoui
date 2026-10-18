#pragma once
#include "cui.hpp"
#include "vec2.hpp"
#include <functional>
#include <memory>

namespace CUI {
    // An element that can receive mouse events, doesn't draw anything
    class HitBoxEl : public Element, public std::enable_shared_from_this<HitBoxEl> {
      public:
        // Whether this hit-box consumes mouse events and occludes hit-boxes behind.
        // If not, a hit-box behind this will still receive mouse events.
        // If false, forces `clickOnDown` true.
        // Default true.
        bool consumes;

        // If true, click event will be generated on mouse down.
        // If false, click event will be generated on mouse up.
        // Forced false when `consumes` is false.
        bool clickOnDown;

        // Called when an event is generated
        std::function<void(MouseEvent)> onEventCallback;

        HitBoxEl(std::function<void(MouseEvent)> onEventCallback = nullptr, bool consumes = true, bool clickOnDown = false)
            : consumes(consumes), clickOnDown(clickOnDown), onEventCallback(onEventCallback) {}

        // Returns true if a point is within this hit-box.
        // Can be overriden by a derived class to define a custom shape.
        virtual bool contains(Vec2 pos);

        // HitBox calls this with the processed events.
        // Can be overriden by a derived class for custom logic.
        // By default calls onEventCallback.
        virtual void onEvent(MouseEvent event) {
            if(onEventCallback) onEventCallback(event);
        }

        void draw(ContextPtr ctx, Rect rect) override;

        // Whether the LMB has been pressed inside this hit-box and still down.
        // Doesn't become false if mouse leaves while still down. If you want this, && it with isHovered()
        bool isPressed();

        // Whether the mouse is inside this hit-box right now.
        bool isHovered() { return hoverPrevious; }

        std::weak_ptr<Context> ctx;
    private:
        Rect lastRect;
        bool hoverNow = false, hoverPrevious = false;

        // Engine sends unprocessed events here. Return true to consume the event. Do NOT consume "up".
        virtual bool processEvent(ContextPtr ctx, MouseEvent event); 
        // Called by the engine once all mouse events are processed
        virtual void postEvents(ContextPtr ctx);

        friend class Context;
    };

    namespace Factories {
        /// An element that can receive mouse events, doesn't draw anything
        /// @param onEventCallback A function that receives medium mouse events. Refer to MouseEvent documentation. It receives ALL events, not just click.
        /// @param consumes 
        /// Whether this hit-box consumes mouse events and occludes hit-boxes behind it.
        /// If not, a hit-box behind this will still receive mouse events.
        /// If false, forces `clickOnDown` true.
        /// Default true.
        /// @param clickOnDown 
        /// If true, click event will be generated on mouse down.
        /// If false, click event will be generated on mouse up.
        /// Forced false when `consumes` is false.
        inline std::shared_ptr<HitBoxEl> HitBox(std::function<void(MouseEvent)> onEventCallback, bool consumes = true, bool clickOnDown = false) {
            return std::make_shared<HitBoxEl>(onEventCallback, consumes, clickOnDown);
        }
    }
}