#pragma once
#include "cui.hpp"
#include "vec2.hpp"
#include <functional>
#include <memory>

namespace CUI {
    class HitBoxEl : public Element, public std::enable_shared_from_this<HitBoxEl> {
      public:
        bool consumes, clickOnDown;
        std::function<void(MouseEvent)> onEventCallback;

        HitBoxEl(std::function<void(MouseEvent)> onEventCallback = nullptr, bool consumes = true, bool clickOnDown = false)
            : consumes(consumes), clickOnDown(clickOnDown), onEventCallback(onEventCallback) {}


        virtual bool contains(Vec2 pos);

        // HitBox calls this with the processed events
        virtual void onEvent(MouseEvent event) {
            if(onEventCallback) onEventCallback(event);
        }

        void draw(ContextPtr ctx, Rect rect) override;
        bool isPressed();
        bool isHovered() { return hoverPrevious; }

    private:
        Rect lastRect;
        bool hoverNow = false, hoverPrevious = false;
        std::weak_ptr<Context> ctx;

        // Engine sends unprocessed events here. Return true to consume the event. Do NOT consume "up".
        virtual bool processEvent(ContextPtr ctx, MouseEvent event); 
        // Called by the engine once all mouse events are processed
        virtual void postEvents(ContextPtr ctx);

        friend class Context;
    };

    namespace Factories {
        inline std::shared_ptr<HitBoxEl> HitBox(std::function<void(MouseEvent)> onEventCallback, bool consumes = true, bool clickOnDown = false) {
            return std::make_shared<HitBoxEl>(onEventCallback, consumes, clickOnDown);
        }
    }
}