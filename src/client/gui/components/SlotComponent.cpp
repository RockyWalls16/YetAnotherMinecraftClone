#include <client/gui/components/SlotComponent.h>
#include <client/input/GameController.h>
#include <client/gui/Gui.h>

SlotComponent::SlotComponent(Gui & parent, Block * block) : parent(parent), block(block), hovered(false)
{
}

void SlotComponent::init()
{
}

void SlotComponent::render()
{
}

void SlotComponent::setPosition(int x, int y)
{
}

void SlotComponent::onInput(int mX, int mY)
{
	if (mX >= x && mY >= y && mX <= x + 16 && mY <= y + 16)
	{
		hovered = true;
		parent.onInputEvent(this);
	}
	else if (hovered)
	{
		hovered = false;
		parent.onInputEvent(this);
	}
}

bool SlotComponent::isHovered()
{
	return hovered;
}

Block * SlotComponent::getBlock()
{
	return block;
}
