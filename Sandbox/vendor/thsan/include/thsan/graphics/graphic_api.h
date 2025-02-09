#pragma once
#include "thsan/core.h"

namespace Thsan {
	enum class THSAN_API GraphicAPI {
		none = 0,
		openGL = 1,
		vulkan = 2
	};
	
	 inline THSAN_API GraphicAPI get_graphic_API();

	class Graphics {
	public:
		Graphics() = delete;
	private:
		static GraphicAPI graphicAPI;
		friend GraphicAPI get_graphic_API();
	};
}