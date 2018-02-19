#ifndef CLIENT_RENDER_FONT_FONTVAO_H_
#define CLIENT_RENDER_FONT_FONTVAO_H_

#include <client/render/util/VertexArray.h>
#include <client/render/font/Font.h>

class FontVAO
{
private:
	Font* font;
	VertexArray* fontVao;
	int vertexAmount;
	int width;
	int height;

public:
	FontVAO(Font* font, VertexArray* fontVao, int vertexAmount, int width, int height);

	~FontVAO();

	void render2D(int x, int y);
};

#endif