#include "pch.h"
#include <thsan/graphics/post_process.h>

#include <thsan/graphics/post_process_effect/snowfall_post_processor.h>
#include <thsan/graphics/post_process_effect/letterbox_post_processor.h>
#include <iostream>

namespace Thsan 
{
    std::unique_ptr<PostProcessorManager> PostProcessorManager::create()
    {
        return std::make_unique<PostProcessorManagerImpl>();
    }

    std::shared_ptr<PostProcessor> PostProcessorManager::createSnowFallPostProcress()
    {
        return std::make_shared<SnowfallPostProcessor>();
    }

    std::shared_ptr<PostProcessor> PostProcessorManager::createLetterBoxPostProcress()
    {
        return std::make_shared<LetterboxPostProcessor>();
    }

    void PostProcessorManagerImpl::add(std::shared_ptr<PostProcessor> processor, float opacity) {
        processors_.emplace_back(processor, opacity);
    }

    void PostProcessorManagerImpl::display() {
        for (const auto& [processor, opacity] : processors_) {
            std::cout << "Applying effect: " << static_cast<int>(processor->GetEffectType()) << std::endl;
            processor->apply(out, opacity);
        }
    }

    void PostProcessorManagerImpl::clear() {
        processors_.clear();
    }

    void PostProcessorManagerImpl::setProcessorOrder(const std::vector<size_t>& order) {
        std::vector<std::pair<std::shared_ptr<PostProcessor>, float>> orderedProcessors;
        for (size_t index : order) {
            if (index < processors_.size()) {
                orderedProcessors.push_back(processors_[index]);
            }
        }
        processors_ = std::move(orderedProcessors);
    }
    std::shared_ptr<Texture2D> PostProcessorManagerImpl::getOutTexture()
    {
        return out;
    }
}