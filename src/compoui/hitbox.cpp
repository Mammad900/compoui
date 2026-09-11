#include "hitbox.hpp"
#include "cui.hpp"
#include "rect.hpp"

bool CUI::HitBoxEl::contains(Vec2 pos) {
    return lastRect.contains(pos);
}

bool CUI::HitBoxEl::processEvent(ContextPtr ctx, MouseEvent event) {
    event.hitBox = shared_from_this();

    if(event.type == MouseEvent::leave) {
        if(event.position)
            event.relativePosition = *event.position - lastRect.pos;
        ctx->pendingEvents.push_back(event);
        return true;
    }
    
    if(event.type == MouseEvent::up) {
        if(event.position)
            event.relativePosition = *event.position - lastRect.pos;
        ctx->pendingEvents.push_back(event);

        if(ctx->capture.get() == this && event.button == MouseEvent::Button::left) {
            ctx->capture = nullptr;
            if(!contains(*event.position) || clickOnDown) return false;
            event.type = MouseEvent::click;
            ctx->pendingEvents.push_back(event);
        }
        return false;
    }

    if(!event.position) return false;
    event.relativePosition = *event.position - lastRect.pos;

    if(event.type == MouseEvent::enter) {
        ctx->pendingEvents.push_back(event);
        return true;
    }

    if(event.type == MouseEvent::hover) {
        if(!contains(*event.position)) return false;
        hoverNow = true;
        return consumes;
    }

    if(event.type == MouseEvent::down) {
        if(!contains(*event.position)) return false;
        ctx->pendingEvents.push_back(event);

        if(event.button == MouseEvent::Button::left) {
            if(clickOnDown || !consumes) {
                event.type = MouseEvent::click;
                ctx->pendingEvents.push_back(event);
            }
            if(consumes)
                ctx->capture = shared_from_this();
        }
        return consumes;
    }

    if(event.type == MouseEvent::move) {
        if(!contains(*event.position) && !isPressed()) return false;
        ctx->pendingEvents.push_back(event);
        return false;
    }

    return consumes;
}

void CUI::HitBoxEl::postEvents(ContextPtr ctx) {
    if(!hoverPrevious && hoverNow) {
        processEvent(ctx, MouseEvent{
            .type= MouseEvent::enter,
            .buttons= ctx->mouseButtons,
            .position= ctx->mousePosition,
        });
    }
    if(hoverPrevious && !hoverNow) {
        processEvent(ctx, MouseEvent{
            .type= MouseEvent::leave,
            .buttons= ctx->mouseButtons,
            .position= ctx->mousePosition,
        });
    }
    hoverPrevious = hoverNow;
    hoverNow = false;
}

void CUI::HitBoxEl::draw(ContextPtr ctx, Rect rect) {
    Element::draw(ctx, rect);
    this->ctx = ctx;
    lastRect = rect;
    ctx->hitBoxes.push_back(shared_from_this());
}

bool CUI::HitBoxEl::isPressed() {
    return ctx.lock()->capture.get() == this;
}