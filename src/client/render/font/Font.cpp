#include <client/render/font/Font.h>

Font::Font(Texture * fntTex, CharInfo ** infos, int maxCh, int lineHeight, int baseLine) : fontTexture(fntTex), charInfos(infos), maxChar(maxCh), lineHeight(lineHeight), baseLine(baseLine)
{
}

Font::~Font()
{
	for (int i = 0; i < maxChar; i++)
	{
		if (charInfos[i] != nullptr)
		{
			delete(charInfos[i]);
		}
	}
	delete(charInfos);
}

CharInfo * Font::getCharInfo(char ch)
{
	return charInfos[ch];
}

void Font::getUV(CharInfo * ch, float* u, float* v, float* u2, float* v2)
{
	// Get char UV
	*u = ch->x / (float) fontTexture->getTextureWidth();
	*v = ch->y / (float)fontTexture->getTextureHeight();

	*u2 = (ch->x2 - ch->x) / (float)fontTexture->getTextureWidth();
	*v2 = (ch->y2 - ch->y) / (float)fontTexture->getTextureHeight();
}

int Font::getLineHeight()
{
	return lineHeight;
}

int Font::getBaseLine()
{
	return baseLine;
}

void Font::bind()
{
	fontTexture->bind();
}
