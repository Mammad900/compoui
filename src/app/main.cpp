#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../src/compoui/cui.hpp"
#include "../compoui/glad/glad.h"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "src/compoui/color.hpp"
#include "src/compoui/cui.hpp"
#include "src/compoui/elements/layout/align.hpp"
#include "src/compoui/elements/layout/freeform.hpp"
#include "src/compoui/elements/layout/list.hpp"
#include "src/compoui/elements/layout/padding.hpp"
#include "src/compoui/elements/layout/zstack.hpp"
#include "src/compoui/elements/layout/optional.hpp"
#include "src/compoui/elements/primitive/fill.hpp"
#include "src/compoui/elements/primitive/image.hpp"
#include "src/compoui/elements/primitive/text.hpp"
#include "src/compoui/hitbox.hpp"
#include "src/compoui/mouse-event.hpp"
#include "src/compoui/texture.hpp"
#include "src/compoui/vec2.hpp"

using namespace CUI;
using namespace Factories;

std::string events[] = {
    "down",
    "up",
    "move",
    "enter",
    "leave",
    "hover",
    "click"
};

std::shared_ptr<Element> button(std::shared_ptr<Texture> icon, std::function<void()> onClick) {
    std::shared_ptr<Primitive::Fill> fill;
    return (
        ZStack({
            fill = Fill({0, 0, 0, 0.2}),
            Padding(10, (
                Image(icon)
            )),
            HitBox([fill, onClick](MouseEvent ev) {
                if(ev.hitBox->isHovered() && ev.hitBox->isPressed())
                    fill->background = Color(0, 0, 0, 0.5);
                else if(ev.hitBox->isHovered() || ev.hitBox->isPressed())
                    fill->background = Color(0, 0, 0, 0.35);
                else
                    fill->background = Color(0, 0, 0, 0.2);
                
                if(ev.type == MouseEvent::click && onClick)
                    onClick();
            }, true, true)
        }, 1)
    );
}

std::shared_ptr<Texture> texture(sf::Image image) {
    return std::make_shared<Texture>(Vec2{(float)image.getSize().x, (float)image.getSize().y}, image.getPixelsPtr());
}
std::shared_ptr<Texture> texture(std::filesystem::path path) {
    return texture(sf::Image(path));
}

class ImagePositioner : public Element {
public:
    std::shared_ptr<Primitive::Image> image;
    std::shared_ptr<Element> child;
    Vec2 centerPos{0.5, 0.5};
    float scaleTarget = 1, scale = 1;
    bool snap = true;
    bool reached = true;
    std::optional<Vec2> dragStart;

    ImagePositioner(std::shared_ptr<Texture> &tex) {
        image = Image(tex);
        child = ZStack({
            image,
            HitBox([this](MouseEvent ev) {
                if(ev.type == MouseEvent::down && ev.button == MouseEvent::left) {
                    dragStart = ev.position;
                }
                if(ev.type == MouseEvent::move && dragStart && ev.hitBox->isPressed() && ev.position)  {
                    Vec2 d = ev.position.value() - dragStart.value();
                    centerPos += d / ev.hitBox->ctx.lock()->windowSize;
                    dragStart = ev.position;
                    snap = false;
                }
                if(ev.type == MouseEvent::up) {
                    dragStart = std::nullopt;
                }
            })
        }, 0);
    }

    void draw(ContextPtr ctx, CUI::Rect rect) override {
        Vec2 &imageSize = image->texture->size;
        if(snap) {
            centerPos = {0.5, 0.5};
            scaleTarget = std::min({
                rect.size.x / imageSize.x,
                rect.size.y / imageSize.y,
                1.f
            });
        }

        scale-= (scale - scaleTarget) * 10.f * ctx->deltaTime;
        if(std::abs(scale - scaleTarget) < 0.0001 || (snap && reached)) {
            reached = true;
            scale = scaleTarget;
        } else {
            reached = false;
        }

        Vec2 size = imageSize * scale;
        child->draw(ctx, {
            centerPos * ctx->windowSize - size*0.5f,
            size
        });
    }
};

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


int main(int argc, char* argv[]) {
    std::filesystem::path filePath = argc > 1 ? argv[1] : "/home/mammad/Pictures/win11-dark.png";
    sf::Image img(filePath);

    sf::ContextSettings settings;
    settings.majorVersion = 4;   // match the GLSL version you use (330)
    settings.minorVersion = 6;

    sf::RenderWindow window(
        sf::VideoMode(img.getSize()),
        filePath.filename().string(),
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

    auto font = std::make_shared<Font>("/usr/share/fonts/truetype/ubuntu/Ubuntu[wdth,wght].ttf", 20);
    auto tex = texture(img);
    auto zoomOriginalTex = texture("/home/mammad/Downloads/zoom-original.png");
    auto zoomInTex = texture("/home/mammad/Downloads/zoom-in.png");
    auto zoomOutTex = texture("/home/mammad/Downloads/zoom-out.png");
    auto maximizeTex = texture("/home/mammad/Downloads/maximise.png");

    std::shared_ptr<Primitive::Fill> fill;
    auto image = std::make_shared<ImagePositioner>(tex);

    auto root = (
        ZStack({
            image,
            Align(AlignMode::center, AlignMode::end, (
                List(ListOrientation::horizontal, {
                    button(zoomOriginalTex, [image]() {
                        image->scaleTarget = 1;
                        image->snap = false;
                    }),
                    button(zoomInTex, [image]() {
                        image->scaleTarget *= 1.414213562373095; // √2
                        image->snap = false;
                    }),
                    button(zoomOutTex, [image]() {
                        image->scaleTarget *= 0.7071067811865475; // 1/√2
                        image->snap = false;
                    }),
                    button(maximizeTex, [image]() {
                        image->snap = true;
                        image->reached = false;
                    }),
                })
            ))
        }, 0)
    );
    auto ctx = std::make_shared<Context>(window.getSize().x, window.getSize().y, root);


    while (window.isOpen()) {
        while(auto e = window.pollEvent()) {
            if(e->getIf<sf::Event::Closed>()) 
                window.close();
            if(auto resize = e->getIf<sf::Event::Resized>())
                ctx->resize(resize->size.x, resize->size.y);
            if(auto pressed = e->getIf<sf::Event::KeyPressed>()) {
                if(pressed->code == sf::Keyboard::Key::Add) {
                    image->scaleTarget *= 1.414213562373095; // √2
                    image->snap = false;
                } else if(pressed->code == sf::Keyboard::Key::Subtract) {
                    image->scaleTarget *= 0.7071067811865475; // 1/√2
                    image->snap = false;
                }
            }
            onMouseEvent(ctx, e);
        }
        ctx->postEvents();

        window.clear();

        ctx->draw();

        window.display();
    }
}