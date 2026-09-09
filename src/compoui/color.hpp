#pragma once

namespace CUI {
struct Color {
    float r, g, b, a;

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

};

}