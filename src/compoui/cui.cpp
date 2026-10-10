#include "cui.hpp"
#include "glad/glad.h"
#include "hitbox.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <optional>

CUI::Context::Context(uint w, uint h, std::shared_ptr<Element> root) 
    : windowSize(w, h), root(root), lastFrame(std::chrono::steady_clock::now()) {

    // A unit quad. x/y/w/h are applied through the model matrix.
    constexpr float vertices[] = {0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CUI::Context::resize(uint w, uint h) {
    windowSize = Vec2(w, h);
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
}

void CUI::Context::draw() {    
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - lastFrame;
    deltaTime = elapsed.count();
    lastFrame = now;

    root->draw(shared_from_this(), {{0,0}, windowSize});
}

void CUI::Context::mouseEvent(MouseEvent event) {
    switch (event.type) {
        case MouseEvent::Type::move: {
            if(!event.position) return;

            mousePosition = event.position;
            break;
        };

        case MouseEvent::Type::down: {
            if(!event.button) return;

            switch (event.button.value()) {
                case MouseEvent::Button::left:
                    mouseButtons.left = true;
                    break;
                case MouseEvent::Button::right:
                    mouseButtons.right = true;
                    break;
                case MouseEvent::Button::middle:
                    mouseButtons.middle = true;
                    break;
                case MouseEvent::Button::extra1:
                    mouseButtons.extra1 = true;
                    break;
                case MouseEvent::Button::extra2:
                    mouseButtons.extra2 = true;
                    break;
            }
            
            if(event.position)
                mousePosition = event.position;

            break;
        }

        case MouseEvent::Type::up: {
            if(!event.button) return;

            switch (event.button.value()) {
                case MouseEvent::Button::left:
                    mouseButtons.left = false;
                    break;
                case MouseEvent::Button::right:
                    mouseButtons.right = false;
                    break;
                case MouseEvent::Button::middle:
                    mouseButtons.middle = false;
                    break;
                case MouseEvent::Button::extra1:
                    mouseButtons.extra1 = false;
                    break;
                case MouseEvent::Button::extra2:
                    mouseButtons.extra2 = false;
                    break;
            }

            event.buttons = mouseButtons;
            if(event.position)
                mousePosition = event.position;

            break;
        }

        case MouseEvent::Type::leave: {
            mousePosition = std::nullopt;
            event.type = MouseEvent::Type::move;
            break;
        }

        case MouseEvent::Type::hover:
            break;
        
        default:
            return;
    }

    event.buttons = mouseButtons;
    event.position = mousePosition;

    std::shared_ptr<Context> shared = shared_from_this();
    for (unsigned i = hitBoxes.size(); i-- > 0; ) {
        auto &hitBox = hitBoxes[i];
        if(hitBox->processEvent(shared, event)) return;
    }
}

void CUI::Context::postEvents() {
    mouseEvent(MouseEvent{
        .type= MouseEvent::hover,
        .buttons= mouseButtons,
        .position= mousePosition,
    });

    std::shared_ptr<Context> shared = shared_from_this();
    for (auto &hitBox : hitBoxes)
        hitBox->postEvents(shared);

    for (auto &event : pendingEvents) {
        event.hitBox->onEvent(event);
    }

    hitBoxes.clear();
    pendingEvents.clear();
}

void CUI::Context::setClip(std::optional<Rect> rect) {
    clipRect = rect;
    if(rect) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(
            rect->pos.x, 
            windowSize.y - (rect->pos.y + rect->size.y), 
            rect->size.x, 
            rect->size.y);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}