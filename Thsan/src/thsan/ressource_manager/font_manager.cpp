#include "pch.h"
#include "thsan/ressource_manager/font_manager.h"
#include <thsan/graphics/font.h>

namespace RessourceManager {
	FontManager::FontManager()
	{
		//ctor
	}

	std::shared_ptr<Thsan::Font> FontManager::get(const std::string& fntFilePath, const std::string& textureFilePath)
	{
		const auto i = p_fonts.find(fntFilePath);
		if (i != p_fonts.end())
			return i->second;
		else {
			std::shared_ptr<Thsan::Font> p_font = Thsan::Font::create();
			p_font->loadFromFile(fntFilePath, textureFilePath);
			p_fonts.insert({ fntFilePath, p_font });
			return p_font;
		}
	}

	void FontManager::removeUnused()
	{
		for (auto i = p_fonts.begin(); i != p_fonts.end();)
			if (i->second.use_count() == 1)
				i = p_fonts.erase(i);
			else
				++i;
	}


	std::unordered_map<std::string, std::shared_ptr<Thsan::Font>> FontManager::p_fonts;
}
