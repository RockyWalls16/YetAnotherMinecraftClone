#ifndef CLIENT_RENDER_FONT_FONTRENDERER_H_
#define CLIENT_RENDER_FONT_FONTRENDERER_H_

#include <string>
#include <client/render/font/Font.h>

class FontVAO;

class FontRenderer
{
public:
	static FontVAO* makeVao(Font* font, std::string text);
};

#endif