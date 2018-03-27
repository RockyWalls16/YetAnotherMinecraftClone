#include <client/gui/components/SliderComponent.h>
#include <util/Logger.h>
#include <client/render/font/FontCache.h>
#include <client/gui/Gui.h>
#include <client/input/GameController.h>
#include <client/audio/AudioFile.h>
#include <client/audio/AudioManager.h>
#include <math/MathUtil.h>

SliderComponent::SliderComponent(Gui& parent, const std::string & text, int width, Font& font) :
	parent(parent),
	textComponent(text, CENTER, font),
	backgroundBegin(Gui::widgetsTexture, width, 20, 0, 46, (width - 32), 66, 256, 256),
	backgroundEnd(Gui::widgetsTexture, 32, 20, (width - 32), 46, width, 66, 256, 256),
	cursorBegin(Gui::widgetsTexture, 4, 20, 0, 66, 4, 86, 256, 256),
	cursorEnd(Gui::widgetsTexture, 4, 20, 196, 66, 200, 86, 256, 256),
	width(width), value(0.0F), min(0.0F), max(1.0F), grabbed(false)
{
}

SliderComponent::SliderComponent(Gui& parent, const std::string & text, int width) : SliderComponent(parent, text, width, *FontCache::defaultFont)
{
}

void SliderComponent::init()
{
	textComponent.init();
	backgroundBegin.init();
	backgroundEnd.init();
	cursorBegin.init();
	cursorEnd.init();
}

void SliderComponent::render()
{
	backgroundBegin.render();
	backgroundEnd.render();
	cursorBegin.render();
	cursorEnd.render();
	textComponent.render();
}

void SliderComponent::setPosition(int x, int y)
{
	GuiComponent::setPosition(x, y);
	backgroundBegin.setPosition(x, y);
	backgroundEnd.setPosition(x + width - 32, y);

	cursorBegin.setPosition(x + value * (width - 8), y);
	cursorEnd.setPosition(x + value * (width - 8) + 4, y);

	textComponent.setPosition(x + width / 2.0F, y + 10 + textComponent.getHeight() / 1.5F + 1);
}

void SliderComponent::onInput(int mX, int mY)
{
	if (GameController::MOUSE_1_KEY->isNotHeld())
	{
		grabbed = false;
	}

	if (mX >= x && mY >= y && mX <= x + width && mY <= y + 20)
	{
		if (GameController::MOUSE_1_KEY->isPressed())
		{
			AudioManager::getInstance().playSound(AudioFile("Gui/click.ogg"));
			grabbed = true;
		}
	}

	if (grabbed)
	{
		value = MathUtil::clamp((mX - x) / (float) (width), 0.0F, 1.0F);

		setPosition(x, y);
		parent.onInputEvent(this);
	}
}

float SliderComponent::getValue()
{
	return value * (max - min) + min;;
}

void SliderComponent::setValue(float newValue)
{
	value = (newValue - min) / (max - min);
	parent.onInputEvent(this);
}

void SliderComponent::setBounds(float min, float max)
{
	this->min = min;
	this->max = max;
}

void SliderComponent::setText(const std::string& newText)
{
	textComponent.setText(newText);
}
