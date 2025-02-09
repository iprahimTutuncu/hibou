#include "pch.h"
#include "thsan/graphics/compute_shader.h"

#include "thsan/graphics/graphic_api.h"
#include "platform/openGL/gl_compute_shader.h"
#include "tsm/math/camera/abstract_camera.h"
#include <glm/gtc/type_ptr.hpp>

#include "thsan/graphics/texture.h"
#include "thsan/graphics/mesh.h"

namespace Thsan {

	void ComputeShader::setCamera(std::weak_ptr<tsm::AbstractCamera> camera)
	{
		std::shared_ptr<tsm::AbstractCamera> tmp_camera = camera.lock();
		if (tmp_camera)
		{
			setMat4("projection", tmp_camera->getProjection());
			setMat4("view", tmp_camera->getView());
			setVec3("eyePosition", tmp_camera->getPosition());
			setVec3("eyeDirection", tmp_camera->getFront());
		}
		else
			TS_CORE_ERROR("error: in ComputeShader::setCamera, camera.lock() return nullptr.");
	}

	std::string ComputeShader::readFile(std::string path)
	{
		std::ifstream ifs(path);
		std::string str(std::istreambuf_iterator<char>{ifs}, {});
		return str;
	}

	std::shared_ptr<ComputeShader> ComputeShader::create(const std::string& cs_path)
	{
		switch (get_graphic_API())
		{
		case GraphicAPI::openGL: return std::make_shared<GLComputeShader>(cs_path);
		case GraphicAPI::vulkan: return nullptr;
		};

		TS_CORE_ERROR("error: ComputeShader not supported for the current graphic API");
		return nullptr;
	}
}
