#include <client/gui/components/ButtonComponent.h>
#include <util/Logger.h>
#include <client/render/font/FontCache.h>
#include <client/gui/Gui.h>
#include <client/input/GameController.h>

ButtonComponent::ButtonComponent(Gui& parent, const std::string & text, int width, Font& font) : parent(parent),
textComponent(text, CENTER, font),
buttonBegin(Gui::widgetsTexture, width, 20, 0, 66, (width - 32), 86, 256, 256),
buttonEnd(Gui::widgetsTexture, 32, 20, (width - 32), 66, width, 86, 256, 256), width(width), hovered(false)
{
}

ButtonComponent::ButtonComponent(Gui& parent, const std::string & text, int width) : ButtonComponent(parent, text, width, *FontCache::defaultFont)
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
	textComponent.setPosition(x + width / 2.0F, y + 10 + textComponent.getHeight() / 1.5F + 1);
}

void ButtonComponent::onInput(int mX, int mY)
{
	if (mX >= x && mY >= y && mX <= x + width && mY <= y + 20)
	{
		if (GameController::MOUSE_1_KEY->isPressed())
		{
			parent.onInputEvent(this);
		}

		if (!hovered)
		{
			buttonBegin.setUV(0, 86, width - 32, 106, 256, 256);
			buttonEnd.setUV(width - 32, 86, width, 106, 256, 256);
		}
		hovered = true;
	}
	else
	{
		if (hovered)
		{
			buttonBegin.setUV(0, 66, width - 32, 86, 256, 256);
			buttonEnd.setUV(width - 32, 66, width, 86, 256, 256);
		}
		hovered = false;
	}
}

void ButtonComponent::setDisabled(bool disabled)
{
	this->disabled = disabled;

	if (disabled)
	{
		buttonBegin.setUV(0, 46, width - 32, 66, 256, 256);
		buttonEnd.setUV(width - 32, 46, width, 66, 256, 256);
	}
	else
	{

	}
}

bool ButtonComponent::isDisabled()
{
	return disabled;
}
