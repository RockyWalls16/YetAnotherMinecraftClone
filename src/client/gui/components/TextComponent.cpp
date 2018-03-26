#include <client/gui/components/TextComponent.h>
#include <client/render/font/FontRenderer.h>
#include <client/render/font/FontCache.h>
#include <util/Logger.h>

TextComponent::TextComponent(const std::string & text, Font & font) : GuiComponent(), text(text), font(font), fontVao(FontVAO(font))
{

}

TextComponent::TextComponent(const std::string & text) : TextComponent(text, *FontCache::defaultFont)
{
}

void TextComponent::init()
{
	FontRenderer::makeVao(fontVao, font, text);
}

void TextComponent::render()
{
	fontVao.render2D(x, y);
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
