#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "../src/compoui/cui.hpp"
#include "../compoui/glad/glad.h"

using namespace CUI;

std::shared_ptr<Element> makeUI();

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
        }

        window.clear();

        ctx->draw();

        window.display();
    }
}