#include <client/gui/components/ButtonComponent.h>
#include <util/Logger.h>
#include <client/render/font/FontCache.h>
#include <client/gui/Gui.h>

ButtonComponent::ButtonComponent(const std::string & text, int width, Font& font) : textComponent(text, font),
buttonBegin(Gui::widgetsTexture, width, 20, 0.0F, 170.0F / 256.0F, (width - 32) / 256.0F, 190.0F / 256.0F),
buttonEnd(Gui::widgetsTexture, 32, 20, (width - 32) / 256.0F, 170.0F / 256.0F, width / 256.0F, 190.0F / 256.0F), width(width)
{
}

ButtonComponent::ButtonComponent(const std::string & text, int width) : ButtonComponent(text, width, *FontCache::defaultFont)
{
}

void ButtonComponent::init()
{
	textComponent.init();
	buttonBegin.init();
	buttonEnd.init();
}

void ButtonComponent::render()
{
	buttonBegin.render();
	buttonEnd.render();
	textComponent.render();
}

void ButtonComponent::setPosition(int x, int y)
{
	GuiComponent::setPosition(x, y);
	buttonBegin.setPosition(x, y);
	buttonEnd.setPosition(x + width - 32, y);
	textComponent.setPosition(x + width / 2.0F - textComponent.getWidth() / 2.0F, y + 10 + textComponent.getHeight() / 1.5F);
}

void ButtonComponent::onInput(int mX, int mY)
{
	if (mX >= x && mY >= y && mX <= x + width && mY <= y + 20)
	{
		
	}
}
