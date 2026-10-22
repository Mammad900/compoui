#include "../../cui.hpp"
#include <memory>
namespace CUI::Layout {
    // Conditionally renders its child.
    // Natural size is that of its child, unless Mode is 'gone', in which case its zero.
    class Optional : public Element {
    public:
        // The element to possibly render
        ElementPtr child;


        enum Mode {
            // Child is drawn as normal.
            visible,
            // Child is not drawn, but natural size still propagates
            invisible, 
            // Child is not drawn and natural size is zero.
            gone 
        };
        Mode mode = visible;

        Optional(ElementPtr child, Mode mode = visible)
            : child(child), mode(mode) {}

        void draw(CUI::Rect rect) override {
            Element::draw(rect);

            if (mode == visible && child)
                child->draw(rect);
        }

        // Report natural size of the child unless mode is 'gone'.
        Vec2 naturalSize(Vec2 availableSpace) override {
            if (mode == gone || !child) return {0, 0};
            return child->naturalSize(availableSpace);
        }
    };
}

namespace CUI::Factories {
    using OptionalMode = Layout::Optional::Mode;
    
    // Conditionally renders its child.
    // Natural size is that of its child, unless Mode is 'gone', in which case its zero.
    inline std::shared_ptr<Layout::Optional> Optional(ElementPtr child, OptionalMode mode = OptionalMode::visible) {
        return std::make_shared<Layout::Optional>(child, mode);
    }
}