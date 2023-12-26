#pragma once
#include "thsan/core.h"
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/render_states.h"

namespace Thsan {
    class THSAN_API Drawable
    {
    public:
        virtual ~Drawable() = default;

    protected:
        friend class RenderTarget;

        virtual void draw(const RenderTarget& target, RenderStates2D& states) const = 0; 
    };
}