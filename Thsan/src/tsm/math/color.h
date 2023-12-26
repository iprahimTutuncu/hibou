#pragma once
#include <cstdint>
#include "thsan/core.h"
#include "thsan/system/vector.h"
#include <glm/ext/vector_float4.hpp>

namespace tsm {
    class THSAN_API Color {
    public:
        Color();
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        Color(int r, int g, int b, int a = 255);
        Color(float h, float s, float v, uint8_t a = 255);

        uint8_t getRed() const;
        uint8_t getGreen() const;
        uint8_t getBlue() const;
        uint8_t getAlpha() const;
        float getHue() const;
        float getSaturation() const;
        float getValue() const;

        void setRed(uint8_t r);
        void setGreen(uint8_t g);
        void setBlue(uint8_t b);
        void setAlpha(uint8_t a);
        void setRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        void setHSV(float h, float s, float v, uint8_t a = 255);

        glm::vec4 toGlm() const;
        Thsan::vec4f toVec4f() const;

        void log() const;

        static Color fromGlm(const glm::vec4& vec4Color);
        static Color fromVec4f(const Thsan::vec4f& vec4fColor);

    private:
        uint8_t red;   // Red component (0-255)
        uint8_t green; // Green component (0-255)
        uint8_t blue;  // Blue component (0-255)
        uint8_t alpha; // Alpha (transparency) component (0-255)

        float hue;        // Hue (0-360 degrees)
        float saturation; // Saturation (0-1)
        float value;      // Value (brightness) (0-1)

        // Update RGB components based on current HSV values
        void updateRGB();

        // Update HSV components based on current RGB values
        void updateHSV();
    };
}

