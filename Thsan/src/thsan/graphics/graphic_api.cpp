#include "pch.h"
#include "graphic_api.h"

namespace Thsan {

    GraphicAPI get_graphic_API()
    {
        return Graphics::graphicAPI;
    }

    GraphicAPI Graphics::graphicAPI = GraphicAPI::openGL;
}