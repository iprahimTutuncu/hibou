#include "pch.h"
#include "color.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "thsan/log.h"

namespace tsm {
    Color::Color() : red(0), green(0), blue(0), alpha(255) {}

    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha(a) {}

    Color::Color(int r, int g, int b, int a)
        : red(r), green(g), blue(b), alpha(a)
    {
    }

    Color::Color(float h, float s, float v, uint8_t a) {
        setHSV(h, s, v, a);
    }

    // Accessors
    uint8_t Color::getRed() const { return red; }
    uint8_t Color::getGreen() const { return green; }
    uint8_t Color::getBlue() const { return blue; }
    uint8_t Color::getAlpha() const { return alpha; }
    float Color::getHue() const { return hue; }
    float Color::getSaturation() const { return saturation; }
    float Color::getValue() const { return value; }

    // Modifiers
    void Color::setRed(uint8_t r) {
        red = r;
        updateHSV();
    }

    void Color::setGreen(uint8_t g) {
        green = g;
        updateHSV();
    }

    void Color::setBlue(uint8_t b) {
        blue = b;
        updateHSV();
    }

    void Color::setAlpha(uint8_t a) { alpha = a; }

    void Color::setRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        red = r;
        green = g;
        blue = b;
        updateHSV();
    }

    void Color::setHSV(float h, float s, float v, uint8_t a) {
        hue = std::max(0.0f, std::min(360.0f, h));
        saturation = std::max(0.0f, std::min(1.0f, s));
        value = std::max(0.0f, std::min(1.0f, v));
        alpha = a;
        updateRGB();
    }

    glm::vec4 Color::toGlm() const
    {
        return glm::vec4(static_cast<float>(red) / 255.0f,
            static_cast<float>(green) / 255.0f,
            static_cast<float>(blue) / 255.0f,
            static_cast<float>(alpha) / 255.0f
        );
    }

    Thsan::vec4f Color::toVec4f() const
    {
        return Thsan::vec4f{static_cast<float>(red) / 255.0f,
            static_cast<float>(green) / 255.0f,
            static_cast<float>(blue) / 255.0f,
            static_cast<float>(alpha) / 255.0f
        };
    }

    // Print the color
    void Color::log() const {
        TS_CORE_INFO("RGBA: ( {}, {} , {}, {}", static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue), static_cast<int>(alpha));
        TS_CORE_INFO("HSV: {}°, {}, {})", hue, saturation, value);
    }

    Color Color::fromGlm(const glm::vec4& vec4Color)
    {
        uint8_t red = static_cast<uint8_t>(vec4Color.r * 255.0f);
        uint8_t green = static_cast<uint8_t>(vec4Color.g * 255.0f);
        uint8_t blue = static_cast<uint8_t>(vec4Color.b * 255.0f);
        uint8_t alpha = static_cast<uint8_t>(vec4Color.a * 255.0f);

        return Color(red, green, blue, alpha);
    }

    Color Color::fromVec4f(const Thsan::vec4f& vec4fColor)
    {
        uint8_t red = static_cast<uint8_t>(vec4fColor.x * 255.0f);
        uint8_t green = static_cast<uint8_t>(vec4fColor.y * 255.0f);
        uint8_t blue = static_cast<uint8_t>(vec4fColor.z * 255.0f);
        uint8_t alpha = static_cast<uint8_t>(vec4fColor.w * 255.0f);

        return Color(red, green, blue, alpha);
    }

    // Update RGB components based on current HSV values
    void Color::updateRGB() {
        float c = value * saturation;
        float x = c * (1.f - std::abs(fmod(hue / 60.f, 2.f) - 1.f));
        float m = value - c;

        float r, g, b;
        if (hue >= 0 && hue < 60) {
            r = c;
            g = x;
            b = 0;
        }
        else if (hue >= 60 && hue < 120) {
            r = x;
            g = c;
            b = 0;
        }
        else if (hue >= 120 && hue < 180) {
            r = 0;
            g = c;
            b = x;
        }
        else if (hue >= 180 && hue < 240) {
            r = 0;
            g = x;
            b = c;
        }
        else if (hue >= 240 && hue < 300) {
            r = x;
            g = 0;
            b = c;
        }
        else {
            r = c;
            g = 0;
            b = x;
        }

        red = static_cast<uint8_t>((r + m) * 255);
        green = static_cast<uint8_t>((g + m) * 255);
        blue = static_cast<uint8_t>((b + m) * 255);
    }

    // Update HSV components based on current RGB values
    void Color::updateHSV() {
        float r = static_cast<float>(red) / 255.0f;
        float g = static_cast<float>(green) / 255.0f;
        float b = static_cast<float>(blue) / 255.0f;

        float maxVal = std::max(std::max(r, g), b);
        float minVal = std::min(std::min(r, g), b);
        float delta = maxVal - minVal;

        value = maxVal;

        if (maxVal == 0.0f) {
            hue = 0.0f;
            saturation = 0.0f;
        }
        else {
            saturation = delta / maxVal;

            if (delta == 0.0f) {
                hue = 0.0f;
            }
            else if (maxVal == r) {
                hue = 60.0f * fmod((g - b) / delta, 6.0f);
            }
            else if (maxVal == g) {
                hue = 60.0f * ((b - r) / delta + 2.0f);
            }
            else {
                hue = 60.0f * ((r - g) / delta + 4.0f);
            }
        }

        if (hue < 0.0f) {
            hue += 360.0f;
        }
    }
}