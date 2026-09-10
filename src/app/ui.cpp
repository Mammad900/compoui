#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include "src/compoui/color.hpp"
#include "src/compoui/cui.hpp"
#include "src/compoui/elements/layout/align.hpp"
#include "src/compoui/elements/layout/freeform.hpp"
#include "src/compoui/elements/layout/list.hpp"
#include "src/compoui/elements/layout/padding.hpp"
#include "src/compoui/elements/layout/zstack.hpp"
#include "src/compoui/elements/primitive/fill.hpp"
#include "src/compoui/elements/primitive/text.hpp"
#include "src/compoui/hitbox.hpp"

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

    std::shared_ptr<Primitive::Fill> fill;

    return Align(AlignMode::center, (
            ZStack({
                fill = Fill(Color(1, 1, 1, 0.1)),
                Padding(10, (
                    Text("Exit", font, Color::white)
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