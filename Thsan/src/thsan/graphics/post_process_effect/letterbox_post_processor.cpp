#include "pch.h"
#include <thsan/graphics/post_process_effect/letterbox_post_processor.h>
#include <iostream>

namespace Thsan
{
    void LetterboxPostProcessor::setEffectParam(const EffectParams params)
    {

    }

    void LetterboxPostProcessor::apply(std::weak_ptr<Texture2D> out, float opacity) {

        std::cout << "Applying Letterbox effect with opacity: " << opacity << std::endl;

        if (auto texturePtr = out.lock()) 
        {

        }
        else 
        {
            std::cerr << "Failed to lock the output texture." << std::endl;
        }
    }

    void LetterboxPostProcessor::setUp() 
    {

        std::cout << "Setting up Letterbox PostProcessor." << std::endl;
    }

    void LetterboxPostProcessor::cleanUp()
    {

        std::cout << "Cleaning up Letterbox PostProcessor." << std::endl;
    }
}