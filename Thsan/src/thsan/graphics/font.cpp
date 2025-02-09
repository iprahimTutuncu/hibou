#include "pch.h"
#include "thsan/graphics/font.h"
#include "thsan/core.h"
#include "thsan/log.h"
#include <fstream>
#include <iostream>
#include <thsan/ressource_manager/texture2D_manager.h>
#include <nlohmann/json.hpp>
#include <thsan/ressource_manager/font_manager.h>

namespace Thsan
{
    std::shared_ptr<Font> Font::create(const std::string& fntFilePath, const std::string& textureFilePath)
    {
        return RessourceManager::FontManager::get(fntFilePath, textureFilePath);
    }

    std::shared_ptr<Font> Font::create()
    {
        return std::make_shared<FontBitmapImpl>();
    }

    FontBitmapImpl::FontBitmapImpl()
    {
    }

    bool FontBitmapImpl::loadFromFile(const std::string& fntFilePath, const std::string& textureFilePath)
    {
        pTexture = RessourceManager::Texture2DManager::get(textureFilePath, false);
        if (!pTexture)
        {
            TS_CORE_ERROR("In FontBitmapImpl::loadFromFile(..), Failed to load texture: {}", textureFilePath);
            return false;
        }

        std::vector<std::string> lines = readFileLines(fntFilePath);
        fontData = parse(lines);
        return true;
    }

    std::optional<CharInfo> FontBitmapImpl::getCharacter(char32_t codePoint) const
    {
        auto it = fontData.charInfo.find(static_cast<uint32_t>(codePoint));
        if (it != fontData.charInfo.end())
        {
            return it->second;
        }
        return std::nullopt;
    }

    int FontBitmapImpl::getKerning(char32_t first, char32_t second) const
    {
        return fontData.getKerningAmount(first, second);
    }

    Info FontBitmapImpl::getInfo() const
    {
        return fontData.info;
    }

    Common FontBitmapImpl::getCommon() const
    {
        return fontData.common;
    }

    float FontBitmapImpl::getTextWidth(const std::u32string& text) const
    {
        float width = 0.0f;
        int currentX = 0;

        for (char32_t codePoint : text) {
            auto charInfoOpt = getCharacter(codePoint);
            if (charInfoOpt) {
                const CharInfo& charInfo = charInfoOpt.value();
                width += charInfo.xadvance;
            }
        }

        return width;
    }

    float FontBitmapImpl::getTextHeight(const std::u32string& text) const
    {
        int maxHeight = 0;

        for (char32_t codePoint : text) {
            auto charInfoOpt = getCharacter(codePoint);
            if (charInfoOpt) {
                const CharInfo& charInfo = charInfoOpt.value();
                int charBottom = charInfo.height - charInfo.yoffset;
                maxHeight = std::max(maxHeight, charBottom);
            }
        }

        return static_cast<float>(maxHeight);
    }

    std::shared_ptr<Texture2D> FontBitmapImpl::getTexture()
    {
        return pTexture;
    }

    std::vector<std::string> FontBitmapImpl::readFileLines(const std::string& filename) {
        std::vector<std::string> lines;
        std::ifstream file(filename);

        if (!file.is_open()) {
            TS_CORE_ERROR("In FontBitmapImpl::readFileLines(..), Failed to load font: {}", filename);
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    }

    std::vector<std::string> FontBitmapImpl::separateWords(const std::string& input) {
        std::vector<std::string> words;
        std::string word;
        bool inQuotes = false;

        for (char32_t ch : input) {
            if (ch == '"')
                inQuotes = !inQuotes;
            else if (ch == ' ' && !inQuotes) {
                if (!word.empty())
                    words.push_back(word);
                word.clear();
            }
            else
                word += ch;
        }

        if (!word.empty())
            words.push_back(word);

        return words;
    }

    FontData FontBitmapImpl::parse(std::vector<std::string> lines)
    {
        std::string Line;
        std::string Read, Key, Value;

        FontData fontData;

        for (const auto& line : lines) {
            // Skip empty lines
            if (line.empty())
                continue;

            // Separate words in the line
            std::vector<std::string> words = separateWords(line);

            if (words.empty())
                continue;

            const std::string& section = words[0]; // The first word determines the section

            // Process line based on the section
            if (section == "info") {
                for (size_t i = 1; i < words.size(); ++i) {
                    auto pos = words[i].find('=');
                    if (pos != std::string::npos) {
                        std::string key = words[i].substr(0, pos);
                        std::string value = words[i].substr(pos + 1);

                        // Assign values to Info struct
                        if (key == "size") fontData.info.fontSize = std::stoi(value);
                        else if (key == "bitfield") fontData.info.bitField = std::stoi(value);
                        else if (key == "charset") fontData.info.charSet = value;
                        else if (key == "stretchH") fontData.info.stretchH = std::stoi(value);
                        else if (key == "aa") fontData.info.aa = std::stoi(value);
                        else if (key == "padding")
                        {
                            std::vector<int> arrayVal;
                            std::stringstream ss(value);

                            for (int i; ss >> i;) {
                                arrayVal.push_back(i);
                                if (ss.peek() == ',')
                                    ss.ignore();
                            }

                            fontData.info.paddingUp = arrayVal[0];
                            fontData.info.paddingRight = arrayVal[1];
                            fontData.info.paddingDown = arrayVal[2];
                            fontData.info.paddingLeft = arrayVal[3];
                        }
                        else if (key == "spacing")
                        {
                            std::vector<int> arrayVal;
                            std::stringstream ss(value);

                            for (int i; ss >> i;) {
                                arrayVal.push_back(i);
                                if (ss.peek() == ',')
                                    ss.ignore();
                            }

                            fontData.info.spacingHoriz = arrayVal[0];
                            fontData.info.spacingVert = arrayVal[1];
                        }
                        else if (key == "face") fontData.info.fontName = value;
                    }
                }
            }
            else if (section == "common") {
                for (size_t i = 1; i < words.size(); ++i) {
                    auto pos = words[i].find('=');
                    if (pos != std::string::npos) {
                        std::string key = words[i].substr(0, pos);
                        std::string value = words[i].substr(pos + 1);

                        // Assign values to Common struct
                        if (key == "lineHeight") fontData.common.lineHeight = std::stoi(value);
                        else if (key == "base") fontData.common.base = std::stoi(value);
                        else if (key == "scaleW") fontData.common.scaleW = std::stoi(value);
                        else if (key == "scaleH") fontData.common.scaleH = std::stoi(value);
                        else if (key == "pages") fontData.common.pages = std::stoi(value);
                        else if (key == "bitfield") fontData.common.bitField = std::stoi(value);
                        else if (key == "alphaChnl") fontData.common.alphaChnl = std::stoi(value);
                        else if (key == "redChnl") fontData.common.redChnl = std::stoi(value);
                        else if (key == "greenChnl") fontData.common.greenChnl = std::stoi(value);
                        else if (key == "blueChnl") fontData.common.blueChnl = std::stoi(value);
                    }
                }
            }
            else if (section == "page") {
                // Handle page names (assumes pages start at index 1 and contain filenames)
                for (size_t i = 1; i < words.size(); ++i) {
                    auto pos = words[i].find('=');
                    if (pos != std::string::npos) {
                        std::string key = words[i].substr(0, pos);
                        std::string value = words[i].substr(pos + 1);

                        if (key == "file") fontData.pages.pageNames.push_back(value);
                    }
                }
            }
            else if (section == "chars") {
                for (size_t i = 1; i < words.size(); ++i) {
                    auto pos = words[i].find('=');
                    if (pos != std::string::npos) {
                        std::string key = words[i].substr(0, pos);
                        std::string value = words[i].substr(pos + 1);

                        fontData.charCount = std::stoi(value);
                    }
                }
            }
            else if (section == "char") {
                CharInfo charInfo;
                for (size_t i = 1; i < words.size(); ++i) {
                    auto pos = words[i].find('=');
                    if (pos != std::string::npos) {
                        std::string key = words[i].substr(0, pos);
                        std::string value = words[i].substr(pos + 1);

                        if (key == "id") charInfo.id = std::stoi(value);
                        else if (key == "x") charInfo.x = std::stoi(value);
                        else if (key == "y") charInfo.y = std::stoi(value);
                        else if (key == "width") charInfo.width = std::stoi(value);
                        else if (key == "height") charInfo.height = std::stoi(value);
                        else if (key == "xoffset") charInfo.xoffset = std::stoi(value);
                        else if (key == "yoffset") charInfo.yoffset = std::stoi(value);
                        else if (key == "xadvance") charInfo.xadvance = std::stoi(value);
                        else if (key == "page") charInfo.page = std::stoi(value);
                        else if (key == "chnl") charInfo.chnl = std::stoi(value);
                    }
                }

                fontData.charInfo[charInfo.id] = charInfo;
            }
            else if (section == "kernings") {
                for (size_t i = 1; i < words.size(); ++i) {
                    auto pos = words[i].find('=');
                    if (pos != std::string::npos) {
                        std::string key = words[i].substr(0, pos);
                        std::string value = words[i].substr(pos + 1);

                        fontData.kerningCount = std::stoi(value);
                    }
                }
            }
            else if (section == "kerning") {
                KerningPair kerningPair;
                for (size_t i = 1; i < words.size(); ++i) {
                    auto pos = words[i].find('=');
                    if (pos != std::string::npos) {
                        std::string key = words[i].substr(0, pos);
                        std::string value = words[i].substr(pos + 1);

                        if (key == "first") kerningPair.first = std::stoi(value);
                        else if (key == "second") kerningPair.second = std::stoi(value);
                        else if (key == "amount") kerningPair.amount = std::stoi(value);
                    }
                }
                fontData.addKerningPair(kerningPair);
            }
        }

        return fontData;
    }
}