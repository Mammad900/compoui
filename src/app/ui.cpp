#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include "SFML/Graphics/Image.hpp"
#include "src/compoui/color.hpp"
#include "src/compoui/cui.hpp"
#include "src/compoui/elements/layout/align.hpp"
#include "src/compoui/elements/layout/freeform.hpp"
#include "src/compoui/elements/layout/list.hpp"
#include "src/compoui/elements/layout/padding.hpp"
#include "src/compoui/elements/layout/zstack.hpp"
#include "src/compoui/elements/primitive/fill.hpp"
#include "src/compoui/elements/primitive/image.hpp"
#include "src/compoui/elements/primitive/text.hpp"
#include "src/compoui/hitbox.hpp"
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

std::shared_ptr<Element> makeUI() {
    auto font = std::make_shared<Font>("/usr/share/fonts/truetype/ubuntu/Ubuntu[wdth,wght].ttf", 20);
    sf::Image image("/mnt/8b2636d6-887c-4ca7-97a6-17d1aad0903a/factorio/data/core/graphics/icons/close.png");
    auto tex = std::make_shared<Texture>(Vec2{(float)image.getSize().x, (float)image.getSize().y}, image.getPixelsPtr());

    std::shared_ptr<Primitive::Fill> fill;

    return Align(AlignMode::center, (
            ZStack({
                fill = Fill(Color(1, 1, 1, 0.1)),
                Padding(10, (
                    List(10, ListOrientation::horizontal, {
                        Image(tex, Vec2{20, 20}),
                        Text("Exit", font, Color::white),
                    })
                )),
                HitBox([fill](MouseEvent ev){
                    if(ev.hitBox->isHovered() && ev.hitBox->isPressed())
                        fill->background = Color(1, 1, 1, 0.3);
                    else if(ev.hitBox->isHovered() || ev.hitBox->isPressed())
                        fill->background = Color(1, 1, 1, 0.2);
                    else
                        fill->background = Color(1, 1, 1, 0.1);

                    if(ev.type == MouseEvent::click)
                        exit(0);
                })
            }, 1)
    ));
}