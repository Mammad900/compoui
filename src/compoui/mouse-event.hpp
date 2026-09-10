#pragma once

#include "vec2.hpp"
#include <optional>
#include <memory>

namespace CUI {
    class HitBoxEl;

    // A mouse event, can be raw (as sent by window to Context), rare (as sent from Context to HitBox) or medium (as sent from HitBox to event handler)
    struct MouseEvent {
        enum Type {
            // [raw, rare, medium] a mouse button just got pressed
            down,
            // [raw, rare, medium] a mouse button just got released
            up,
            // [raw, rare, medium] the mouse cursor moved
            move,
            // [medium] the mouse cursor entered the hit-box
            enter, 
            // [raw, medium] the mouse cursor left the window/hit-box
            leave, 
            // [rare, medium] the mouse cursor has been on this position this frame
            hover,
            // [medium] the LMB was pressed or released (depending on hit-box settings)
            click
        };

        // The pressed state of mouse buttons
        struct Buttons {
            // Mouse1, LMB, primary
            bool left:1;
            // Mouse2, RMB, secondary
            bool right:1;
            // Mouse3, MMB, scroll wheel
            bool middle:1;
            // Mouse4, forward side button
            bool extra1:1;
            // Mouse5, backward side button
            bool extra2:1;
        };

        // A mouse button
        enum Button {
            // Mouse1, LMB, primary
            left, 
            // Mouse2, RMB, secondary
            right, 
            // Mouse3, MMB, scroll wheel
            middle, 
            // Mouse4, forward side button
            extra1, 
            // Mouse5, backward side button
            extra2,
        };

        // What kind of event happened?
        Type type;

        // The state of mouse buttons after the event. 
        // Ignored for raw.
        Buttons buttons;

        // The button that was pressed/released. 
        // For down, up and click events. Always left button for click event.
        std::optional<Button> button;

        // The position of the mouse cursor relative to window after the event.
        // For raw, required for move, optional for up/down, ignored for others. 
        // For rare/medium, always present unless the cursor is outside the window.
        std::optional<Vec2> position;

        // The position of the mouse cursor relative to the hit-box.
        // Ignored for raw.
        // For rare/medium, present iff position is present.
        std::optional<Vec2> relativePosition;

        // Medium only, the hit-box that sent this event.
        std::shared_ptr<HitBoxEl> hitBox;
    };
}