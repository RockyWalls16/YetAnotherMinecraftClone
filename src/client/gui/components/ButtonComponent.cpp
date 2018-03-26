#include <client/gui/components/ButtonComponent.h>
#include <client/render/font/FontCache.h>
#include <client/gui/Gui.h>

ButtonComponent::ButtonComponent(const std::string & text, int width, Font& font) : textComponent(text, font), buttonBegin(Gui::widgetsTexture, width, 20, 0.0F, 170.0F / 256.0F, (width - 32) / 256.0F, 190.0F / 256.0F)
{
}

ButtonComponent::ButtonComponent(const std::string & text, int width) : ButtonComponent(text, width, *FontCache::defaultFont)
{
}

void ButtonComponent::init()
{
	textComponent.init();
	buttonBegin.init();
}

void ButtonComponent::render()
{
	textComponent.render();
	buttonBegin.render();
}

void ButtonComponent::setPosition(int x, int y)
{
	GuiComponent::setPosition(x, y);
	textComponent.setPosition(x, y);
	buttonBegin.setPosition(x, y);
}
