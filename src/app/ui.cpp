#include <memory>
#include "src/compoui/cui.hpp"
#include "src/compoui/elements/layout/align.hpp"
#include "src/compoui/elements/layout/freeform.hpp"
#include "src/compoui/elements/layout/list.hpp"
#include "src/compoui/elements/layout/padding.hpp"
#include "src/compoui/elements/layout/zstack.hpp"
#include "src/compoui/elements/primitive/fill.hpp"

using namespace CUI;
using namespace Factories;

std::shared_ptr<Element> makeUI() {
    return Freeform({
        {{{10, 10}, {70, 0}}, (
            ZStack({
                Fill({0,1,0,0.5}),
                // Padding({10, 10}, (
                //     Fill({0,0,1,0.5})
                // ))
                List(10, {
                    Align(AlignMode::start, AlignMode::end, (
                        Fill({1,0,0,1}, {30,15})
                    )),
                    Align(AlignMode::center, AlignMode::end, (
                        Fill({1,0,0,1}, {30,15})
                    )),
                    Align(AlignMode::end, AlignMode::end, (
                        Fill({1,0,0,1}, {30,15})
                    )),
                }),
            }, 1)
        )},
        // {{{-1, 10}, {50, 50}}, (
        //     Fill({0,0,1,0.5})
        // )},
    });
}