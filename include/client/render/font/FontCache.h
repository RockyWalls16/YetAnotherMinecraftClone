#ifndef CLIENT_RENDER_FONT_FONTCACHE_H_
#define CLIENT_RENDER_FONT_FONTCACHE_H_

#include <client/render/font/Font.h>
#include <string>
#include <vector>

class FontCache
{
private:
	static std::vector<Font*> loadedFonts;

public:
	static Font* defaultFont;

	static void init();
	static Font* loadFont(std::string name);

	static void cleanUp();
};

#endif