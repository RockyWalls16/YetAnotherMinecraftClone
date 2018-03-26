#ifndef CLIENT_RENDER_FONT_FONT_H_
#define CLIENT_RENDER_FONT_FONT_H_

#include <client/textures/Texture.h>

// Contains character size
class CharInfo
{
public:
	unsigned char character;
	int x;
	int y;
	int x2;
	int y2;
	int offsetX;
	int offsetY;
	int xAdvance;

	CharInfo(unsigned char character, int x, int y, int x2, int y2, int offX, int offY, int xAdvance) : character(character), x(x), y(y), x2(x2), y2(y2), offsetX(offX), offsetY(offY), xAdvance(xAdvance) {}
};

class Font
{
private:
	Texture* fontTexture;
	CharInfo* charInfos[256]{ nullptr };
	int maxChar;
	int lineHeight;
	int baseLine;

public:
	Font(Texture* fntTex, int maxCh, int lineHeight, int baseLine);
	~Font();

	CharInfo* getCharInfo(unsigned char ch);

	void getUV(CharInfo* ch, float* u, float* v, float* u2, float* v2);

	int getLineHeight();

	int getBaseLine();

	void bind();

	CharInfo** getCharInfoArray();
};

#endif