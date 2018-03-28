#include <client/gui/GuiInventory.h>
#include <client/textures/TextureLoader.h>

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

	inventorySlots = new SlotComponent*[12];
	for (int i = 0; i < 12; i++)
	{
		inventorySlots[i] = new SlotComponent(*this, Block::getBlockList()[i]);
		addComponent(inventorySlots[i]);
	}
}

GuiInventory::~GuiInventory()
{
	for (int i = 0; i < 12; i++)
	{
		delete(inventorySlots[i]);
	}
	delete(inventorySlots);
}

void GuiInventory::prepareLayout()
{
	inventoryTitle.setPosition(width / 2 - (195 / 2) + 8, height / 2 + (136 / 2) - 4);
	inventoryBackground.setPosition(width / 2 - (195 / 2), height / 2 - (136 / 2));

	for (int i = 0; i < 12; i++)
	{
		int row = i / 9;
		int column = i % 9;
		inventorySlots[i]->setPosition(width / 2 - (195 / 2) + 8 + column * 18, height / 2 + (136 / 2) - 4 + row * 18);
	}
}

void GuiInventory::onInputEvent(GuiComponent * component)
{
	SlotComponent* slot = dynamic_cast<SlotComponent*>(component);
	if (slot)
	{
		if (slot->isHovered())
		{
			inventoryTitle.setText("Inventory - " + slot->getBlock()->getName());
		}
		else
		{
			inventoryTitle.setText("Inventory");
		}
	}
	else
	{
		inventoryTitle.setText("Inventory");
	}
}
