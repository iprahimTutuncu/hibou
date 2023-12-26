#include "pch.h"
#include "sprite_animation.h"
#include "texture.h"
#include "mesh.h"
#include "tsm/math/transform.h"
#include "render_states.h"
#include "thsan/log.h"
#include "shader.h"

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
        unsigned short getCurrIndex() { return curr; }
        unsigned short getSize() { return max; }
        float getTotalDuration() { return totalDuration; }
        float getReachedDuration() { return reachedDuration; }
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
        bool isLoop() { return looping; }

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

        mesh = create_mesh(4);
        std::shared_ptr<Thsan::Mesh> quadMesh = Thsan::create_mesh(4);

        // Create vertices for the quad
        std::vector<Thsan::Vertex> vertices(4);

        // Vertex 0 - Top Left
        vertices[0].position = Thsan::vec3f{ 0.f, 0.f, 0.0f };
        vertices[0].color = this->color.toVec4f();
        vertices[0].texCoord = Thsan::vec2f{ 0.f , 0.0f };

        // Vertex 1 - Top Right
        vertices[1].position = Thsan::vec3f{ 1.f, 0.f, 0.0f };
        vertices[1].color = this->color.toVec4f();
        vertices[1].texCoord = Thsan::vec2f{ 1.0f, 0.0f };

        // Vertex 2 - Bottom Right
        vertices[2].position = Thsan::vec3f{ 1.f, 1.f, 0.0f };
        vertices[2].color = this->color.toVec4f();
        vertices[2].texCoord = Thsan::vec2f{ 1.0f, 1.0f };

        // Vertex 3 - Bottom Left
        vertices[3].position = Thsan::vec3f{ 0.f, 1.f, 0.0f };
        vertices[3].color = this->color.toVec4f();
        vertices[3].texCoord = Thsan::vec2f{ 0.0f, 1.0f };

        // Set the quad vertices
        quadMesh->setVertices(vertices);

        // Create indices for the quad (two triangles)
        std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3 };
        quadMesh->setIndices(indices);

        mesh->generate();


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
        Thsan::vec4f col = color.toVec4f();
        // Vertex 0 - Top Left
        Mesh& mesh_ref = *mesh;
        mesh_ref[0].position = Thsan::vec3f{ 0.f, 0.f, 0.0f };
        mesh_ref[0].color = col;
        mesh_ref[0].texCoord = Thsan::vec2f{ coord.x , coord.y };

        // Vertex 1 - Top Right
        mesh_ref[1].position = Thsan::vec3f{ rect.z, 0.f, 0.0f };
        mesh_ref[1].color = col;
        mesh_ref[1].texCoord = Thsan::vec2f{ coord.x + coord.z, coord.y };

        // Vertex 2 - Bottom Right
        mesh_ref[2].position = Thsan::vec3f{ rect.z, rect.w, 0.0f };
        mesh_ref[2].color = col;
        mesh_ref[2].texCoord = Thsan::vec2f{ coord.x + coord.z, coord.y + coord.w };

        // Vertex 3 - Bottom Left
        mesh_ref[3].position = Thsan::vec3f{ 0.f, rect.w, 0.0f };
        mesh_ref[3].color = col;
        mesh_ref[3].texCoord = Thsan::vec2f{ coord.x, coord.y + coord.w };

        std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3 };
        mesh_ref.setIndices(indices);

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

    void SpriteAnimationImpl::hide()
    {
        hidden = true;
    }

    void SpriteAnimationImpl::show()
    {
        hidden = false;
    }

    bool SpriteAnimationImpl::isHidden() const
    {
        return hidden;
    }

    void SpriteAnimationImpl::flipX()
    {
        xFlip *= -1;
    }

    void SpriteAnimationImpl::flipY()
    {
        yFlip *= -1;
    }

    bool SpriteAnimationImpl::isFlipX() const
    {
        if (xFlip == 1)
            return false;
        return true;
    }

    bool SpriteAnimationImpl::isFlipY() const
    {
        if (yFlip == 1)
            return false;
        return true;
    }
    void SpriteAnimationImpl::setTexture(std::shared_ptr<Texture2D> texture)
    {
        if (texture) {
            Thsan::vec2u size = texture->getSize();
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

    void SpriteAnimationImpl::reset()
    {
        stopped = true;
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

    int SpriteAnimationImpl::getFrameWidth()
    {
        return animations[curr]->getCurrentFrameRect().z;
    }

    int SpriteAnimationImpl::getFrameHeight() 
    {
        return animations[curr]->getCurrentFrameRect().w;
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

     void SpriteAnimationImpl::draw(const RenderTarget& target, RenderStates2D& states) const
    {
         auto s = states.getShader().lock();
         s->setVec4("key_color", keyColor.toGlm());
         s->setMat4("transform", transform->getTransform());
         s->setTexture2D("default_texture", texture);
         mesh->bind();
         target.draw(*mesh, states);
         mesh->unbind();
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
         transform->scale(scale);
     }

     glm::vec3 SpriteAnimationImpl::getScale() const
     {
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
     glm::mat4 SpriteAnimationImpl::getTranform()
     {
         return transform->getTransform();
     }
}
