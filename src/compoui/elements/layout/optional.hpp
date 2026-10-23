#include "../../cui.hpp"
#include "value/value.hpp"
#include <memory>
namespace CUI::Layout {
    // Conditionally renders its child.
    // Natural size is that of its child.
    class Optional : public Element {
    public:
        // The element to possibly render
        ElementPtr child;

        Val<bool> visible;

        Optional(ElementPtr child, Val<bool> visible)
            : child(child), visible(visible) {}

        void draw(CUI::Rect rect) override {
            Element::draw(rect);

            if (visible.get() && child)
                child->draw(rect);
        }

        // Report natural size of the child unless mode is 'gone'.
        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }
    };
}

namespace CUI::Factories {
    // Conditionally renders its child.
    // Natural size is that of its child, unless Mode is 'gone', in which case its zero.
    inline std::shared_ptr<Layout::Optional> Optional(Val<bool> visible, ElementPtr child) {
        return std::make_shared<Layout::Optional>(child, visible);
    }
}