#include "pch.h"
#include <thsan/graphics/post_process_effect/snowfall_post_processor.h>
#include <iostream>

namespace Thsan
{
    void SnowfallPostProcessor::setEffectParam(const EffectParams params)
    {

    }

    void SnowfallPostProcessor::apply(std::weak_ptr<Texture2D> out, float opacity) {

        std::cout << "Applying SnowfallPostProcessor effect with opacity: " << opacity << std::endl;

        if (auto texturePtr = out.lock())
        {

        }
        else
        {
            std::cerr << "Failed to lock the output texture." << std::endl;
        }
    }

    void SnowfallPostProcessor::setUp()
    {

        std::cout << "Setting up SnowfallPostProcessor PostProcessor." << std::endl;
    }

    void SnowfallPostProcessor::cleanUp()
    {

        std::cout << "Cleaning up SnowfallPostProcessor PostProcessor." << std::endl;
    }
}