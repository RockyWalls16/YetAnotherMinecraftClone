#include <client/gui/components/TextComponent.h>
#include <client/render/font/FontRenderer.h>
#include <client/render/font/FontCache.h>
#include <util/Logger.h>

TextComponent::TextComponent(const std::string & text, TextAlign horizontalAlign, Font & font) : GuiComponent(), text(text), horizontalAlign(horizontalAlign), font(font), fontVao(FontVAO(font))
{

}

TextComponent::TextComponent(const std::string & text, TextAlign horizontalAlign) : TextComponent(text, horizontalAlign, *FontCache::defaultFont)
{
}

void TextComponent::init()
{
	FontRenderer::makeVao(fontVao, font, text);
}

void TextComponent::render()
{
	int rX = x;
	int rY = y;

	if (horizontalAlign == CENTER)
	{
		rX = x - (fontVao.getWidth() / 2);
	}
	else if (horizontalAlign == RIGHT)
	{
		rX = x - fontVao.getWidth();
	}

	fontVao.render2D(rX, rY);
}

void TextComponent::setText(const std::string & newText)
{
	if (newText != text)
	{
		text = newText;
		init();
	}
}

int TextComponent::getWidth()
{
	return fontVao.getWidth();
}

int TextComponent::getHeight()
{
	return fontVao.getHeight();
}
