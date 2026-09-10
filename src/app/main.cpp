#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "../src/compoui/cui.hpp"
#include "../compoui/glad/glad.h"

using namespace CUI;

std::shared_ptr<Element> makeUI();

void onMouseEvent(ContextPtr ctx, const std::optional<sf::Event> &event) {
    if(const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
        ctx->mouseEvent(MouseEvent{
            .type = MouseEvent::move,
            .position = Vec2{(float)moved->position.x, (float)moved->position.y},
        });
        return;
    }
    if(const auto* down = event->getIf<sf::Event::MouseButtonPressed>()) {

        ctx->mouseEvent(MouseEvent{
            .type = MouseEvent::down,
            .button = (MouseEvent::Button)down->button,
            .position = Vec2{(float)down->position.x, (float)down->position.y},
        });
        return;
    }
    if(const auto* up = event->getIf<sf::Event::MouseButtonReleased>()) {
        ctx->mouseEvent(MouseEvent{
            .type = MouseEvent::up,
            .button = (MouseEvent::Button)up->button,
            .position = Vec2{(float)up->position.x, (float)up->position.y},
        });
        return;
    }

    if(event->getIf<sf::Event::MouseLeft>()) {
        ctx->mouseEvent(MouseEvent{
            .type = MouseEvent::leave,
        });
        return;
    }
}


int main() {
    sf::ContextSettings settings;
    settings.majorVersion = 4;   // match the GLSL version you use (330)
    settings.minorVersion = 6;

    sf::RenderWindow window(
        sf::VideoMode({300, 300}),
        "Test App",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );
    // *** IMPORTANT ***
    // The SFML context must be current before loading GL functions.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        std::cerr << "Failed to initialize OpenGL loader!\n";
        return EXIT_FAILURE;
    }

    auto ctx = std::make_shared<Context>(window.getSize().x, window.getSize().y, makeUI());


    while (window.isOpen()) {
        while(auto e = window.pollEvent()) {
            if(e->getIf<sf::Event::Closed>()) 
                window.close();
            if(auto resize = e->getIf<sf::Event::Resized>())
                ctx->resize(resize->size.x, resize->size.y);
            onMouseEvent(ctx, e);
        }
        ctx->postEvents();

        window.clear();

        ctx->draw();

        window.display();
    }
}