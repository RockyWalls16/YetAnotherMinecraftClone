#include <client/render/font/FontCache.h>
#include <fstream>
#include <regex>
#include <util/Logger.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/Texture.h>

#define FONT_PATH "Assets/Fonts/"

std::vector<Font*> FontCache::loadedFonts = std::vector<Font*>();
Font* FontCache::defaultFont = nullptr;

void FontCache::init()
{
	defaultFont = loadFont("default");
}

Font * FontCache::loadFont(std::string name)
{
	// Get inputstream
	std::string path = FONT_PATH + name + ".fnt";
	std::ifstream fontFile(path);

	if (fontFile.good())
	{
		fontFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			int maxCharId = 0;
			int charAmount = 0;
			int charWriteIndex = 0;
			int lineHeight = 0;
			int baseLine = 0;
			CharInfo** charInfos = nullptr;
			Texture* fontTexture = nullptr;

			int i = 1;
			for (std::string line; std::getline(fontFile, line); i++)
			{
				// Read line height
				if (i == 2)
				{
					std::regex pattern{ "\\bcommon lineHeight=([^ ]*)\\s*base=([^ ]*)\\s+.*" };
					std::smatch match;

					if (!std::regex_search(line, match, pattern))
					{
						Error("Bad file format");
						return nullptr;
					}

					lineHeight = std::atoi(((std::string) match[1]).c_str());
					baseLine = std::atoi(((std::string) match[2]).c_str());
				}
				else if (i == 3) // Read texture file
				{
					std::regex pattern{ "\\bfile=\"([^ ]*).png\"" };
					std::smatch match;

					if (!std::regex_search(line, match, pattern))
					{
						Error("Bad file format");
						return nullptr;
					}

					fontTexture = TextureLoader::loadTexture("Fonts/" + (std::string) match[1]);
					if (fontTexture == nullptr)
					{
						Error("Font texture not found");
						return nullptr;
					}
				}
				else if (i == 4)
				{
					// Read char count
					std::regex pattern{ "\\bchars count=([^ ]*)" };
					std::smatch match;

					if (!std::regex_search(line, match, pattern))
					{
						Error("Bad file format");
						return nullptr;
					}

					charAmount = std::atoi(((std::string) match[1]).c_str());
					charInfos = new CharInfo*[charAmount];
				}
				else if(i > 4)
				{
					std::regex pattern{ "char id=([^ ]*)\\s*x=([^ ]*)\\s*y=([^ ]*)\\s*width=([^ ]*)\\s*height=([^ ]*)\\s*xoffset=([^ ]*)\\s*yoffset=([^ ]*)\\s*xadvance=([^ ]*)\\s+.*" };
					std::smatch match;

					if (!std::regex_search(line, match, pattern))
					{
						break;
					}

					// Register char
					char charId = (char) std::atoi(((std::string) match[1]).c_str());
					int x = std::atoi(((std::string) match[2]).c_str());
					int y = std::atoi(((std::string) match[3]).c_str());
					int w = std::atoi(((std::string) match[4]).c_str());
					int h = std::atoi(((std::string) match[5]).c_str());
					int offX = std::atoi(((std::string) match[6]).c_str());
					int offY = std::atoi(((std::string) match[7]).c_str());
					int xAdvance = std::atoi(((std::string) match[8]).c_str());

					// Flip Y coordinates (Since V coordinates start from bottom)
					y = fontTexture->getTextureHeight() - y - h;

					charInfos[charWriteIndex++] = new CharInfo(charId, x, y, x + w, y + h, offX, offY, xAdvance);

					if (charId > maxCharId)
					{
						maxCharId = charId;
					}
				}
			}

			// Reorganize array for faster access
			CharInfo** charInfoSorted = new CharInfo*[maxCharId + 1]{nullptr};
			for (int c = 0; c < charAmount; c++)
			{
				charInfoSorted[charInfos[c]->character] = charInfos[c];
			}

			// Create font & add to cache
			Font* font = new Font(fontTexture, charInfoSorted, maxCharId + 1, lineHeight, baseLine);
			loadedFonts.push_back(font);

			// Clean & close
			delete(charInfos);
			fontFile.close();

			return font;
		}
		catch (std::ifstream::failure& e)
		{
			Error("Failed to load font from: " + path + " " + e.what());
		}
	}
	else
	{
		Error("Font file not found: " + path);
	}

	return nullptr;
}

void FontCache::cleanUp()
{
	Info("Cleaning up fonts...");
	for (Font* font : loadedFonts)
	{
		delete(font);
	}
	loadedFonts.clear();
}
