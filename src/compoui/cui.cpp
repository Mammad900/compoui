#include "cui.hpp"
#include "elements/primitive/image.hpp"
#include "glad/glad.h"
#include "hitbox.hpp"
#include "elements/layer.hpp"
#include "fbo.hpp"
#include "texture.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <optional>

using namespace CUI;

thread_local Context* CUI::ctx = nullptr;

Context::Context(uint w, uint h, ElementPtr root) 
    : Context(w, h, Factories::Layer(root)) {}
Context::Context(uint w, uint h, std::shared_ptr<Misc::Layer> root) 
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
    glBlendFuncSeparate(
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, // RGB
        GL_ONE, GL_ONE_MINUS_SRC_ALPHA    // alpha
    );
}

void Context::resize(uint w, uint h) {
    windowSize = Vec2(w, h);
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

    fboPool.clear();
    mainFBO = std::make_shared<FBO>();
}

void Context::draw() {    
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - lastFrame;
    deltaTime = elapsed.count();
    lastFrame = now;

    if(!mainFBO) {
        mainFBO = std::make_shared<FBO>();
    }
    
    activateFBO(mainFBO);

    glClearColor(background.r, background.g, background.b, background.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glBlendFuncSeparate(
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, // RGB
        GL_ONE, GL_ONE_MINUS_SRC_ALPHA    // alpha
    );
    glViewport(0, 0, (int)windowSize.x, (int)windowSize.y);

    root->draw({{0,0}, windowSize});
}

void Context::display() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    pasteFBO(mainFBO);
}

void Context::mouseEvent(MouseEvent event) {
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

    for (unsigned i = hitBoxes.size(); i-- > 0; ) {
        auto &hitBox = hitBoxes[i];
        if(hitBox->processEvent(event)) return;
    }
}

void Context::postEvents() {
    mouseEvent(MouseEvent{
        .type= MouseEvent::hover,
        .buttons= mouseButtons,
        .position= mousePosition,
    });

    for (auto &hitBox : hitBoxes)
        hitBox->postEvents();

    for (auto &event : pendingEvents) {
        event.hitBox->onEvent(event);
    }

    hitBoxes.clear();
    pendingEvents.clear();
}

void Context::setClip(std::optional<Rect> rect) {
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

void Context::pasteFBO(std::shared_ptr<FBO> &src) {
    static std::shared_ptr<Texture> texture;
    if(!texture) texture = std::make_shared<Texture>(Vec2{0,0}, 0);
    static std::shared_ptr<Primitive::Image> img;
    if(!img) img = Factories::Image(texture);

    texture->id = src->tex;
    img->draw({{0,windowSize.y}, windowSize * Vec2{1, -1}});

    glBlendFuncSeparate(
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, // RGB
        GL_ONE, GL_ONE_MINUS_SRC_ALPHA    // alpha
    );
}

void Context::activateFBO(std::shared_ptr<FBO> &fbo, bool clear) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);
    currentFBO = fbo;

    if(clear) {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void PendingLayer::draw() {
    PendingLayer *parentLayer = ctx->currentLayer;
    ctx->currentLayer = this;

    root->draw(rect);

    for (auto &&child : children) {
        child.draw();
    }

    ctx->currentLayer = parentLayer;
}

std::shared_ptr<FBOLease> Context::getFBO() {
    for (auto &&fbo : fboPool) {
        if(!fbo->busy) return fbo->lease();
    }

    // No FBO found
    std::shared_ptr<FBO> newFbo = std::make_shared<FBO>();
    fboPool.push_back(newFbo);
    return newFbo->lease();
}