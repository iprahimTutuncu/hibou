#include "pch.h"
#include "thsan/graphics/drawable/text.h"
#include "thsan/graphics/font.h"
#include "thsan/graphics/mesh.h"
#include "thsan/log.h"
#include "tsm/math/transform.h"
#include "tsm/math/color.h"
#include "thsan/graphics/shader.h"

namespace Thsan {

    std::shared_ptr<Text> Thsan::Text::create()
    {
        return std::make_shared<TextImpl>();
    }

    TextImpl::TextImpl()
    {
        mesh = Mesh::create(4);
        transform = tsm::Transform::create();
    }

    void TextImpl::setString(const std::u32string& text)
    {
        this->text = text;

        if (this->text == U"")
        {
        }

        mesh->clear();

        if (!pFont) {
            TS_CORE_ERROR("Error dans TextImpl::setString, Brooooo, t'as pas set le font haha, tu veux que je fasse quoi? set un font pls.");
            return;
        }
        // If the text is empty, render a transparent quad
        if (this->text.empty())
        {
            // Define a transparent color (assuming RGBA)
            glm::vec4 transparentColor(0.0f, 0.0f, 0.0f, 0.0f);

            // Define a quad covering the desired area (e.g., 1x1 unit quad)
            std::vector<Vertex> vertices = {
                { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.f), glm::vec3(0.f), transparentColor },
                { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.f), glm::vec3(0.f), transparentColor },
                { glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.f), glm::vec3(0.f), transparentColor },
                { glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.f), glm::vec3(0.f), transparentColor }
            };

            std::vector<uint32_t> indices = {
                0, 1, 2,
                2, 3, 0
            };

            mesh->setVertices(vertices);
            mesh->setIndices(indices);
        }
        else {
            const Common& common = pFont->getCommon();
            float lineHeight = static_cast<float>(common.lineHeight);
            float base = static_cast<float>(common.base);

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            float cursorX = 0.0f;
            float cursorY = 0.0f;
            float maxY = 0.0f;

            for (size_t i = 0; i < text.size(); ++i) {
                char32_t currentChar = text[i];
                char32_t nextChar = (i + 1 < text.size()) ? text[i + 1] : '\0';

                // Get character info
                auto characterOpt = pFont->getCharacter(currentChar);
                if (!characterOpt) continue; // Skip if character not found

                const CharInfo& character = *characterOpt;

                // Handle kerning
                int kerning = 0;
                if (nextChar != '\0')
                {
                    kerning = pFont->getKerning(currentChar, nextChar);
                }

                // Calculate vertices
                float x = cursorX + character.xoffset;
                float y = cursorY + character.yoffset;
                float w = character.width;
                float h = character.height;
                float u0 = static_cast<float>(character.x) / common.scaleW;
                float v0 = static_cast<float>(character.y) / common.scaleH;
                float u1 = static_cast<float>(character.x + character.width) / common.scaleW;
                float v1 = static_cast<float>(character.y + character.height) / common.scaleH;


                glm::vec3 normal(0.0f, 0.0f, 1.0f); // Assuming a 2D plane facing the camera

                // Add vertices for the current character
                vertices.push_back({ glm::vec3(x, y, 0.0f), normal, glm::vec2(u0, v0), glm::vec3(0.f), glm::vec3(0.f), fontColor });
                vertices.push_back({ glm::vec3(x + w, y, 0.0f), normal, glm::vec2(u1, v0) , glm::vec3(0.f), glm::vec3(0.f), fontColor });
                vertices.push_back({ glm::vec3(x + w, y + h, 0.0f), normal, glm::vec2(u1, v1), glm::vec3(0.f), glm::vec3(0.f), fontColor });
                vertices.push_back({ glm::vec3(x, y + h, 0.0f), normal, glm::vec2(u0, v1) , glm::vec3(0.f), glm::vec3(0.f), fontColor });

                // Add indices for the current character
                uint32_t vertexOffset = static_cast<uint32_t>(vertices.size()) - 4;
                indices.push_back(vertexOffset + 0);
                indices.push_back(vertexOffset + 1);
                indices.push_back(vertexOffset + 2);
                indices.push_back(vertexOffset + 2);
                indices.push_back(vertexOffset + 3);
                indices.push_back(vertexOffset + 0);

                // Update cursor position
                cursorX += character.xadvance + kerning;
            }

            mesh->setVertices(vertices);
            mesh->setIndices(indices);

            mesh->generateTangents();
            mesh->generate();
        }

        textWidth = pFont->getTextWidth(text);
        textHeight = pFont->getTextHeight(text);
    }

    const std::u32string TextImpl::getString() const
    {
        return text;
    }

    void TextImpl::draw(const RenderTarget& target, RenderStates& states) const
    {
        states.setTexture(TextureType::Default, pFont->getTexture());

        auto s = states.getShader().lock();
        s->setVec4("keyColor", tsm::Color(0.f, 0.f, 0.f, 0u).toGlm());
        s->setMat4("transform", transform->getTransform());
        s->setMat4("rotation", transform->getRotation());

        if (states.getRendererType() == RendererType::rendererTerrain)
        {
            //s->setVec3("directionFromCamera", ? ? );
        }

        mesh->bind();
        target.draw(*mesh, states);
        mesh->unbind();

        s->setMat4("transform", glm::identity<glm::mat4>());
        s->setVec4("keyColor", tsm::Color(0.f, 0.f, 0.f, 0u).toGlm()); //have a const value tsm::transparent instead
    }

    void TextImpl::setFont(std::shared_ptr<Font> font)
    {
        pFont = font;
    }

    void TextImpl::setPosition(const glm::vec3& position)
    {
        transform->setTranslation(position);
    }

    glm::vec3 TextImpl::getPosition() const
    {
        return transform->getTranslation();
    }

    void TextImpl::setRotation(const glm::vec3& rotate, float angle)
    {
        transform->setRotation(rotate, angle);
    }

    glm::mat4 TextImpl::getRotation() const
    {
        return transform->getRotation();
    }

    void TextImpl::setScale(const glm::vec3& scale)
    {
        transform->scale(scale);
    }

    glm::vec3 TextImpl::getScale() const
    {
        return transform->getScale();
    }

    void TextImpl::setOrigin(const glm::vec3& origin)
    {
        transform->setOrigin(origin);
    }

    glm::vec3 TextImpl::getOrigin() const
    {
        return transform->getOrigin();
    }

    void TextImpl::move(const glm::vec3& offset)
    {
        transform->translate(offset);
    }

    void TextImpl::scale(const glm::vec3& scaleFactor)
    {
        transform->scale(scaleFactor);
    }

    void TextImpl::rotate(const glm::vec3& axis, float angleDegrees)
    {
        transform->rotate(axis, angleDegrees);
    }

    std::shared_ptr<Mesh> TextImpl::getMesh() const
    {
        return mesh;
    }

    glm::mat4 TextImpl::getTransform()
    {
        return transform->getTransform();
    }

    void TextImpl::setColor(tsm::Color color)
    {
        fontColor = color.toGlm();
    }

    tsm::Color TextImpl::getColor()
    {
        return tsm::Color::fromGlm(fontColor);
    }

    float TextImpl::getTextWidth() const
    {
        return textWidth;
    }

    float TextImpl::getTextHeight() const
    {
        return textHeight;
    }
}