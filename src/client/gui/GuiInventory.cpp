#include <client/gui/GuiInventory.h>
#include <client/input/GameController.h>
#include <client/textures/TextureLoader.h>
#include <Game.h>
#include <core/entity/EntityPlayer.h>

Texture* GuiInventory::inventoryTexture = nullptr;

GuiInventory::GuiInventory() :
	inventoryTitle("Inventory", LEFT),
	inventoryBackground(GuiInventory::inventoryTexture, 195, 136, 0, 0, 195, 136, 256, 256)
{
	if (!inventoryTexture)
	{
		inventoryTexture = TextureLoader::loadTexture("Gui/inventory");
		inventoryBackground.setTexture(inventoryTexture);
	}

	addComponent(&inventoryBackground);
	addComponent(&inventoryTitle);

	creativeSlots = new SlotComponent*[Block::getBlockList().size() - 1];
	for (int i = 1; i < Block::getBlockList().size(); i++)
	{
		creativeSlots[i - 1] = new SlotComponent(*this, i + 100, Block::getBlockList()[i]);
		addComponent(creativeSlots[i - 1]);
	}

	playerInventory = new SlotComponent*[9];
	for (int i = 0; i < 9; i++)
	{
		playerInventory[i] = new SlotComponent(*this, i, Game::getInstance().getPlayer()->getInventory()[i]);
		addComponent(playerInventory[i]);
	}
}

GuiInventory::~GuiInventory()
{
	for (int i = 0; i < Block::getBlockList().size() - 1; i++)
	{
		delete(creativeSlots[i]);
	}
	delete(creativeSlots);
}

void GuiInventory::prepareLayout()
{
	inventoryTitle.setPosition(width / 2 - (195 / 2) + 8, height / 2 + (136 / 2) - 4);
	inventoryBackground.setPosition(width / 2 - (195 / 2), height / 2 - (136 / 2));

	for (int i = 0; i < Block::getBlockList().size() - 1; i++)
	{
		int row = i / 9;
		int column = i % 9;
		creativeSlots[i]->setPosition(width / 2 - (195 / 2) + 9 + column * 18, (height / 2 + (136 / 2) - 4) - row * 18 - 30);
	}

	for (int i = 0; i < 9; i++)
	{
		playerInventory[i]->setPosition(width / 2 - (195 / 2) + 9 + i * 18, (height / 2 + (136 / 2) - 4) - 124);
	}
}

void GuiInventory::onInputEvent(GuiComponent * component)
{

}

void GuiInventory::onInputUpdate(int mX, int mY)
{
	Gui::onInputUpdate(mX, mY);

	if (GameController::E_KEY->isPressed())
	{
		close();
	}
}

void GuiInventory::onSlotHoverChange(SlotComponent& slot)
{
	if (slot.isHovered())
	{
		inventoryTitle.setText("Inventory - " + slot.getBlock()->getName());
	}
	else
	{
		inventoryTitle.setText("Inventory");
	}
}

void GuiInventory::onSlotChange(SlotComponent & slot)
{
	if (slot.id < 9)
	{
		Game::getInstance().getPlayer()->getInventory()[slot.id] = slot.slotBlock;
	}
}
