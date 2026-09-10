#pragma once

namespace CUI {
    struct Color {
        float r, g, b, a;

        constexpr Color(float r = 0, float g = 0, float b = 0, float a = 1) : r(r), g(g), b(b), a(a) {}

        static Color mix(Color x, Color y, float a) { return x * (1 - a) + y * a; }
        static Color mix(Color x, Color y, Color a) { return x * (Color{1,1,1,1} - a) + y * a; }

        Color operator+(const Color &other) const {
            return {r + other.r, g + other.g, b + other.b, a + other.a};
        }
        Color operator-(const Color &other) const {
            return {r - other.r, g - other.g, b - other.b, a - other.a};
        }
        constexpr Color &operator+=(const Color &right) {
            r += right.r;
            g += right.g;
            b += right.b;
            a += right.a;
            return *this;
        }
        constexpr Color &operator-=(const Color &right) {
            r -= right.r;
            g -= right.g;
            b -= right.b;
            a -= right.a;
            return *this;
        }
        constexpr Color &operator*=(const Color &right) {
            r *= right.r;
            g *= right.g;
            b *= right.b;
            a *= right.a;
            return *this;
        }
        constexpr Color &operator*=(const float &right) {
            r *= right;
            g *= right;
            b *= right;
            a *= right;
            return *this;
        }
        Color operator*(const Color &other) const {
            return {r * other.r, g * other.g, b * other.b, a * other.a};
        }
        Color operator/(const Color &other) const {
            return {r / other.r, g / other.g, b / other.b, a / other.a};
        }
        Color operator*(float scalar) const {
            return {r * scalar, g * scalar, b * scalar, a * scalar};
        }
        Color operator/(float scalar) const {
            return {r / scalar, g / scalar, b / scalar, a / scalar};
        }
        Color operator+(float scalar) const {
            return {r + scalar, g + scalar, b + scalar, a + scalar};
        }
        Color operator-(float scalar) const {
            return {r - scalar, g - scalar, b - scalar, a - scalar};
        }
        bool operator==(const Color &other) const {
            return (r == other.r && g == other.g && b == other.b && a == other.a);
        }
        // operator float() const { return a; }

        static const Color black, white, red, green, blue, yellow, magenta, cyan, transparent;
    };

    inline constexpr Color Color::black(0, 0, 0);
    inline constexpr Color Color::white(255, 255, 255);
    inline constexpr Color Color::red(255, 0, 0);
    inline constexpr Color Color::green(0, 255, 0);
    inline constexpr Color Color::blue(0, 0, 255);
    inline constexpr Color Color::yellow(255, 255, 0);
    inline constexpr Color Color::magenta(255, 0, 255);
    inline constexpr Color Color::cyan(0, 255, 255);
    inline constexpr Color Color::transparent(0, 0, 0, 0);
}
