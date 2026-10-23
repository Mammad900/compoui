#include "value.hpp"
#include <type_traits>

namespace CUI {
    namespace Value {
        /// Returns the specified member of a swappable struct. The struct member should be of type Val<T>.
        /// @tparam T Type of the value
        /// @tparam S The class/struct which property to get
        template<typename T, typename S>
            requires (std::is_class_v<S>)
        class ThemeVar : public Value<T> {
        public:
            S*& theme; 
            Val<T> S::* var;

            ThemeVar(S*& theme, Val<T> S::* var) : theme(theme), var(var) {}

            const T& get() {
                return (theme->*var).get();
            }
        };
    }
}