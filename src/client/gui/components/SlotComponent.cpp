#include <client/gui/components/SlotComponent.h>
#include <client/input/GameController.h>
#include <client/textures/TextureCache.h>
#include <client/gui/Gui.h>

SlotComponent* SlotComponent::draggedSlot = nullptr;

SlotComponent::SlotComponent(Gui & parent, int id, Block * block) : parent(parent), id(id)
, slotTexture(TextureCache::blockTexture, 16, 16, 0, 0, 16, 16, 256, 256)
, slotBlock(block), hovered(false)
{
	int texture = block->getTexture(Side::NORTH);
	int textureRow = texture / 16;
	int textureColumn = texture % 16;
	slotTexture.setUV(textureColumn * 16, textureRow * 16, textureColumn * 16 + 16, textureRow * 16 + 16, 256, 256);
}

SlotComponent::SlotComponent(const SlotComponent & other) : parent(other.parent), slotTexture(TextureCache::blockTexture, 16, 16, 0, 0, 16, 16, 256, 256)
{
	id = other.id;
	slotBlock = other.slotBlock;
}

void SlotComponent::init()
{
	int texture = slotBlock->getTexture(Side::NORTH);
	int textureRow = texture / 16;
	int textureColumn = texture % 16;
	slotTexture.setUV(textureColumn * 16, textureRow * 16, textureColumn * 16 + 16, textureRow * 16 + 16, 256, 256);

	slotTexture.init();
}

void SlotComponent::render()
{
	if (slotBlock->isVisible())
	{
		slotTexture.render();
	}
}

void SlotComponent::setPosition(int x, int y)
{
	GuiComponent::setPosition(x, y);
	slotTexture.setPosition(x, y);
}

void SlotComponent::onInput(int mX, int mY)
{
	if (draggedSlot == this)
	{
		setPosition(mX - 8, mY - 8);
		return;
	}

	if (mX >= x && mY >= y && mX <= x + 16 && mY <= y + 16)
	{
		hovered = true;
		parent.onSlotHoverChange(*this);

		if (GameController::MOUSE_1_KEY->isPressed())
		{
			// Pick slot
			if (!draggedSlot)
			{
				Pick();
			}
			else
			{
				// Swap
				if (slotBlock)
				{
					Swap();
				}
				else // Place
				{
					Place();
				}
			}
		}
	}
	else if (hovered)
	{
		hovered = false;
		parent.onSlotHoverChange(*this);
	}
}

bool SlotComponent::isHovered()
{
	return hovered;
}

Block * SlotComponent::getBlock()
{
	return slotBlock;
}

void SlotComponent::Pick()
{
	draggedSlot = new SlotComponent(*this);
	parent.addComponent(draggedSlot);
	slotBlock = Block::AIR;
	init();
	parent.onSlotChange(*this);
}

void SlotComponent::Swap()
{
	Block* swap = slotBlock;
	slotBlock = draggedSlot->slotBlock;
	draggedSlot->slotBlock = swap;
	init();
	draggedSlot->init();
	parent.onSlotChange(*this);
}

void SlotComponent::Place()
{
	slotBlock = draggedSlot->slotBlock;
	draggedSlot->parent.removeComponent(draggedSlot);
	init();
	delete(draggedSlot);
	SlotComponent::draggedSlot = nullptr;
	parent.onSlotChange(*this);
}
