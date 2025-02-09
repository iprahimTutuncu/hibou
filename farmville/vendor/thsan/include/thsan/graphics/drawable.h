#pragma once
#include "thsan/core.h"
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/render_states.h"

namespace Thsan {
    class THSAN_API Drawable {
    public:
        virtual ~Drawable() = default;
        virtual std::shared_ptr<Mesh> getMesh() const = 0;

        void hide() { hidden = true; }
        void show() { hidden = false; }
        bool isHidden() const { return hidden; }

    protected:
        friend class RenderTarget;
        virtual void draw(const RenderTarget& target, RenderStates& states) const = 0;

    private:
        bool hidden {false};
    };
}
