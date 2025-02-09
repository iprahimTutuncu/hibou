#pragma once

#include "thsan/core.h"
#include <string>

/*

    Font is only in bitmap. No freeType or actualy fancy scalable thing.
    why? I felt like it and I'm lazy to do it. I kinda like using bitmap.
    I use Hiero to generate the bitmap. I support the BTMFont

*/

namespace RessourceManager
{
    class FontManager;
}

namespace Thsan
{
    class InputStream;
    class Texture2D;

    struct Info
    {
        uint16_t fontSize;
        uint8_t bitField;
        std::string charSet;
        uint16_t stretchH;
        int aa;
        int paddingUp;
        int paddingRight;
        int paddingDown;
        int paddingLeft;
        int spacingHoriz;
        int spacingVert;
        std::string fontName;
    };

    struct Common
    {
        uint16_t lineHeight;
        uint16_t base;
        uint16_t scaleW;
        uint16_t scaleH;
        uint16_t pages;
        uint8_t bitField;
        uint8_t alphaChnl;
        uint8_t redChnl;
        uint8_t greenChnl;
        uint8_t blueChnl;
    };

    struct Pages
    {
        std::vector<std::string> pageNames;
    };

    struct CharInfo
    {
        uint32_t id;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        int16_t xoffset;
        int16_t yoffset;
        int16_t xadvance;
        uint8_t page;
        uint8_t chnl;
    };

    struct KerningPair
    {
        uint32_t first;
        uint32_t second;
        int16_t amount;
    };

    struct FontData
    {
        int charCount{ 0 };
        int kerningCount{ 0 };

        Info info;
        Common common;
        Pages pages;
        std::unordered_map<uint32_t, CharInfo> charInfo;
    private:
        std::unordered_map<uint64_t, int16_t> kerningPair;
    public:
        uint64_t makeKey(uint32_t first, uint32_t second) const
        {
            return (static_cast<uint64_t>(first) << 32) | second;
        }

        void addKerningPair(const KerningPair& pair)
        {
            uint64_t key = makeKey(pair.first, pair.second);
            kerningPair[key] = pair.amount;
        }

        int16_t getKerningAmount(uint32_t first, uint32_t second) const
        {
            uint64_t key = makeKey(first, second);
            auto it = kerningPair.find(key);
            if (it != kerningPair.end())
            {
                return it->second;
            }
            return 0; // 0 c'est qui a pas de kerning
        }

    };

    /*
        font.cpp(101,25): warning C4244: 'argument': conversion from 'char32_t' to '_Elem', possible loss of data
        si jamais c'est la hess, check ca. peut-être faudra utiliser std::u32string.
    */

    class THSAN_API Font
    {
    public:
        virtual ~Font() = default;

        Font() = default;
        Font(const Font& copy) {};

        virtual bool loadFromFile(const std::string& fntFilePath, const std::string& textureFilePath) = 0;

        virtual std::optional<CharInfo> getCharacter(char32_t codePoint) const = 0;

        virtual int getKerning(char32_t first, char32_t second) const = 0;

        virtual Info getInfo() const = 0;

        virtual Common getCommon() const = 0;

        virtual float getTextWidth(const std::u32string& text) const = 0;

        virtual float getTextHeight(const std::u32string& text) const = 0;

        virtual std::shared_ptr<Texture2D> getTexture() = 0;

        static std::shared_ptr<Font> create(const std::string& fntFilePath, const std::string& textureFilePath);
    private:
        static std::shared_ptr<Font> create();
        friend class RessourceManager::FontManager;

    };

    class FontBitmapImpl : public Font
    {
    public:

    public:
        FontBitmapImpl();
        ~FontBitmapImpl() = default;

        bool loadFromFile(const std::string& fntFilePath, const std::string& textureFilePath) override;
        std::optional<CharInfo> getCharacter(char32_t codePoint) const override;
        int getKerning(char32_t first, char32_t second) const override;
        Info getInfo() const override;
        Common getCommon() const override;
        float getTextWidth(const std::u32string& text) const override;
        float getTextHeight(const std::u32string& text) const override;
        std::shared_ptr<Texture2D> getTexture() override;

    private:
        FontData fontData;
        std::shared_ptr<Texture2D> pTexture;

    private:
        std::vector<std::string> readFileLines(const std::string& filename);
        std::vector<std::string> separateWords(const std::string& input);
        FontData parse(std::vector<std::string> lines);
    };


}
