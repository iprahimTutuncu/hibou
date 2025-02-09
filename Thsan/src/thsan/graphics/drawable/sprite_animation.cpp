#include "pch.h"
#include "thsan/log.h"
#include "thsan/graphics/drawable/sprite_animation.h"
#include "thsan/graphics/texture.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/render_states.h"
#include "thsan/graphics/shader.h"
#include "tsm/math/transform.h"

namespace Thsan {

    class AnimationData {
    public:
        AnimationData() :curr(0), max(0), totalDuration(0.f), reachedDuration(0.f) {};
        glm::vec4 getCurrentFrameRect() { return frameRect[curr]; };
        float getCurrentFrameTime() { return frameTime[curr]; };

        void addFrame(glm::vec4 rect, float time) {
            frameRect.emplace_back(rect);
            frameTime.emplace_back(time);
            max++;
            totalDuration += time;
        };
        void reset() {
            curr = 0;
            reachedDuration = 0.f;
        }
        void setCurrIndex(unsigned short i) { curr = i; }
        unsigned short getCurrIndex() const { return curr; }
        unsigned short getSize() const { return max; }
        float getTotalDuration() const { return totalDuration; }
        float getReachedDuration() const { return reachedDuration; }
        void nextFrame() {
            if (curr >= max - 1) {
                if (looping)
                    curr = 0;
                else
                    reachedDuration = 0.f;
            }
            else
                curr++;
        };
        void prevFrame() { (curr < 0) ? curr = max - 1 : curr--; }
        void enableLoop(bool isEnable) { looping = isEnable; }
        bool isLoop() const { return looping; }

    private:
        std::vector<glm::vec4> frameRect;
        std::vector<float> frameTime;
        unsigned short curr;
        unsigned short max;
        float totalDuration;
        float reachedDuration;
        bool looping{ true };
    };


    SpriteAnimationImpl::SpriteAnimationImpl()
    {
        color.setRGB(255, 255, 255, 255);
        keyColor.setRGB(0, 0, 0, 0);
        mesh = Mesh::create(4);
        transform = tsm::Transform::create();
    }

    void SpriteAnimationImpl::generate()
    {
        auto& data = *animations[curr];
        glm::vec4 rect = data.getCurrentFrameRect();
        glm::vec4 coord = rect;

        coord.x /= spritesheet_width;
        coord.y /= spritesheet_height;
        coord.z /= spritesheet_width;
        coord.w /= spritesheet_height;
        glm::vec4 col = color.toGlm();

        float halfWidth = rect.z / 2.0f;
        float halfHeight = rect.w / 2.0f;

        // Vertex 0 - Top Left
        Mesh& mesh_ref = *mesh;
        mesh_ref[0].position = glm::vec3{ -halfWidth, -halfHeight, 0.0f };
        mesh_ref[0].color = col;
        mesh_ref[0].texCoord = glm::vec2{ coord.x, coord.y };

        // Vertex 1 - Top Right
        mesh_ref[1].position = glm::vec3{ halfWidth, -halfHeight, 0.0f };
        mesh_ref[1].color = col;
        mesh_ref[1].texCoord = glm::vec2{ coord.x + coord.z, coord.y };

        // Vertex 2 - Bottom Right
        mesh_ref[2].position = glm::vec3{ halfWidth, halfHeight, 0.0f };
        mesh_ref[2].color = col;
        mesh_ref[2].texCoord = glm::vec2{ coord.x + coord.z, coord.y + coord.w };

        // Vertex 3 - Bottom Left
        mesh_ref[3].position = glm::vec3{ -halfWidth, halfHeight, 0.0f };
        mesh_ref[3].color = col;
        mesh_ref[3].texCoord = glm::vec2{ coord.x, coord.y + coord.w };
        mesh_ref.setIndices({ 0, 1, 2, 0, 2, 3 });
        mesh_ref.generate();
    }



    std::shared_ptr<SpriteAnimation> SpriteAnimation::create()
    {
        return std::make_shared<SpriteAnimationImpl>();
    }

    std::shared_ptr<SpriteAnimation> SpriteAnimation::create(std::shared_ptr<Texture2D> texture)
    {
        std::shared_ptr<SpriteAnimation> sa = std::make_shared<SpriteAnimationImpl>();
        sa->setTexture(texture);
        return sa;
    }

    void SpriteAnimationImpl::flipX()
    {
        xFlip *= -1.f;
    }

    void SpriteAnimationImpl::flipY()
    {
        yFlip *= -1.f;
    }

    bool SpriteAnimationImpl::isFlipX() const
    {
        if (xFlip == 1.f)
            return false;
        return true;
    }

    bool SpriteAnimationImpl::isFlipY() const
    {
        if (yFlip == 1.f)
            return false;
        return true;
    }
    void SpriteAnimationImpl::setTexture(std::shared_ptr<Texture2D> texture)
    {
        if (texture) {
            glm::uvec2 size = texture->getSize();
            spritesheet_width = static_cast<float>(size.x);
            spritesheet_height = static_cast<float>(size.y);
            this->texture = texture;
        }
        else
        {
            TS_CORE_WARN("in SpriteAnimationImpl::setTexture(std::shared_ptr<Texture2D> texture), texture is mullptr.");
        }
    }

    void SpriteAnimationImpl::setCurrAnimation(std::string name)
    {
        if(!hasAnimation(name))
            animations[name] = std::make_unique<AnimationData>();

        curr = name;
    }

    void SpriteAnimationImpl::add(glm::vec4 rect, float time)
    {
        animations[curr]->addFrame(rect, time);
    }

    void SpriteAnimationImpl::enableLoop()
    {
        animations[curr]->enableLoop(true);
    }

    void SpriteAnimationImpl::disableLoop()
    {
        animations[curr]->enableLoop(false);
    }

    void SpriteAnimationImpl::start(unsigned int from, unsigned int to)
    {
        stopped = false;
        onNewFrame = true;
        this->from = from;
        this->to = to;
        animations[curr]->setCurrIndex(from);
    }

    void SpriteAnimationImpl::stop()
    {
        stopped = true;
    }

    void SpriteAnimationImpl::resetCurrAnimation()
    {
        tempsAccumuleAnimation = 0.f;
        animations[curr]->reset();
        animations[curr]->setCurrIndex(from);
    }

    void SpriteAnimationImpl::restart(unsigned int from, unsigned int to)
    {
        stopped = true;
        start(from, to);
    }

    bool SpriteAnimationImpl::isFinished() const
    {
        return animationIsDone;
    }

    void SpriteAnimationImpl::setSpeed(float speed_factor)
    {
        this->speedFactor = speed_factor;
    }

    void SpriteAnimationImpl::update(const float& dt) 
    {

        //especially for the loop stuff, have a from and to logic.

        if (stopped)
            return;

        if (animations[curr]->getSize() == 0)
            return;
        else {
            if (tempsAccumuleFrame > animations[curr]->getCurrentFrameTime()) {
                animations[curr]->nextFrame();
                onNewFrame = true;
                tempsAccumuleFrame = 0.f;
            }

            if (tempsAccumuleAnimation > animations[curr]->getTotalDuration()) {
                tempsAccumuleAnimation = 0.f;
                if (!animations[curr]->isLoop())
                    animationIsDone = true;
            }

            tempsAccumuleFrame += speedFactor * dt;

            if (!animationIsDone) {
                tempsAccumuleAnimation += speedFactor * dt;
            }
        }

        if (onNewFrame)
        {
            generate();
            onNewFrame = false;
        }

    }

    bool SpriteAnimationImpl::hasAnimation(const std::string& name) const
    {
        if (animations.find(name) != animations.end())
            return true;
        return false;
    }

    unsigned int SpriteAnimationImpl::getFrameIndex() 
    {
        return animations[curr]->getCurrIndex();
    }

    int SpriteAnimationImpl::getFrameCount()
    {
        return animations[curr]->getSize();  // la quantité de frame
    }

    float SpriteAnimationImpl::getFrameWidth() const
    {
        return animations.at(curr)->getCurrentFrameRect().z;
    }

    float SpriteAnimationImpl::getFrameHeight() const
    {
        return animations.at(curr)->getCurrentFrameRect().w;
    }

    std::string SpriteAnimationImpl::getCurrentAnimation()
    {
        return curr;
    }

    float SpriteAnimationImpl::getCurrentAnimationTimeDuration()
    {
        return animations[curr]->getTotalDuration();
    }

    float SpriteAnimationImpl::getCurrentAnimationTimeReached() const
    {
        return tempsAccumuleAnimation;
    }

    glm::uvec2 SpriteAnimationImpl::getSpriteSheetSize() const 
    {
        auto size = texture->getSize();
        return glm::vec2(size.x, size.y);
    }

    void SpriteAnimationImpl::setColor(tsm::Color color)
    {
        this->color = color;
    }

    tsm::Color SpriteAnimationImpl::getColor() const
    {
        return color;
    }

    void SpriteAnimationImpl::setKeyColor(tsm::Color color)
    {
        keyColor = color;
    }

    tsm::Color SpriteAnimationImpl::getKeyColor() const
    {
        return keyColor;
    }


     void SpriteAnimationImpl::draw(const RenderTarget& target, RenderStates& states) const
    {
         states.setTexture(TextureType::Default, texture);

         auto s = states.getShader().lock();
         s->setVec4("keyColor", keyColor.toGlm());
         s->setMat4("transform", transform->getTransform());
         s->setMat4("rotation", transform->getRotation());
         s->setVec2("extScale", this->extScale);
         s->setVec3("extPosition", this->extPosition);
         s->setFloat("frameHeight", getFrameHeight());

         if (states.getRendererType() == RendererType::rendererTerrain)
         {
           //  s->setVec3("directionFromCamera", ?? );
         }


         mesh->bind();

         target.draw(*mesh, states);

         mesh->unbind();

         s->setMat4("transform", glm::identity<glm::mat4>());
         s->setVec4("keyColor", tsm::Color(0.f,0.f,0.f, 0u).toGlm()); //have a const value tsm::transparent instead
    }

     void SpriteAnimationImpl::setPosition(const glm::vec3& position)
     {
         transform->setTranslation(position);
     }

     glm::vec3 SpriteAnimationImpl::getPosition() const
     {
         return transform->getTranslation();
     }

     void SpriteAnimationImpl::setRotation(const glm::vec3& rotate, float angle)
     {
         transform->setRotation(rotate, angle);
     }

     glm::mat4 SpriteAnimationImpl::getRotation() const
     {
         return transform->getRotation();
     }

     void SpriteAnimationImpl::setScale(const glm::vec3& scale)
     {
         transform->setScale(scale);
     }

     glm::vec3 SpriteAnimationImpl::getScale() const
     {
         return transform->getScale();
         return transform->getScale();
     }

     void SpriteAnimationImpl::setOrigin(const glm::vec3& origin)
     {
         transform->setOrigin(origin);
     }

     glm::vec3 SpriteAnimationImpl::getOrigin() const
     {
         return transform->getOrigin();
     }

     void SpriteAnimationImpl::setExternalTransform(glm::vec3 extPosition, glm::vec2 scale, float rotation)
     {
         float w = getFrameWidth() / 2.f;
         float h = getFrameHeight();

         // Set positions without considering origin 
         (*mesh)[0].externalPosition = glm::vec3(-w/2.f,   h, 0.f);
         (*mesh)[1].externalPosition = glm::vec3( w/2.f,   h, 0.f);
         (*mesh)[2].externalPosition = glm::vec3( w/2.f, 0.f, 0.f);
         (*mesh)[3].externalPosition = glm::vec3(-w/2.f, 0.f, 0.f);

         this->extPosition = extPosition;
         this->extRotation = rotation;
         this->extScale = scale * glm::vec2(xFlip, yFlip);

     }


     glm::vec3 SpriteAnimationImpl::getExternalPosition()
     {
         return extPosition;
     }

     glm::vec2 SpriteAnimationImpl::getExternalScale()
     {
         return extScale;
     }

     float SpriteAnimationImpl::getExternalRotation()
     {
         return extRotation;
     }


     void SpriteAnimationImpl::move(const glm::vec3& offset)
     {
         transform->translate(offset);
     }

     void SpriteAnimationImpl::scale(const glm::vec3& scaleFactor)
     {
         transform->scale(scaleFactor);
     }

     void SpriteAnimationImpl::rotate(const glm::vec3& axis, float angleDegrees)
     {
         transform->rotate(axis, angleDegrees);
     }
     glm::mat4 SpriteAnimationImpl::getTransform()
     {
         return transform->getTransform();
     }
     std::shared_ptr<Texture2D> SpriteAnimationImpl::getTexture()
     {
         return texture;
     }
}
