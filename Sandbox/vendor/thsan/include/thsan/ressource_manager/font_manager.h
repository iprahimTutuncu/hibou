#pragma once

#include <unordered_map>
#include <memory>
#include <string>

namespace Thsan
{
	class Font;
}

namespace RessourceManager {
	
	class FontManager
	{
	public:
		FontManager();
		static std::shared_ptr<Thsan::Font> get(const std::string& fntFilePath, const std::string& textureFilePath);
		static void removeUnused();
	private:
		static std::unordered_map<std::string, std::shared_ptr<Thsan::Font>> p_fonts;
	};
}
