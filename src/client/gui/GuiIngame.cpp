#include <client/gui/GuiIngame.h>
#include <util/TimeManager.h>
#include <Game.h>
#include <client/render/GameRenderer.h>
#include <core/world/World.h>
#include <client/textures/TextureCache.h>
#include <client/gui/components/ButtonComponent.h>

GuiIngame::GuiIngame() : Gui(),
fpsCounter("..."),
renderDebug("..."),
cursor(Gui::widgetsTexture, 16, 16, 240, 0, 256, 16, 256, 256),
inventoryHotbar(TextureComponent(Gui::widgetsTexture, 182, 22, 0, 0, 182, 22, 256, 256)),
inventorySelected(Gui::widgetsTexture, 24, 24, 0, 22, 24, 46, 256, 256)
{
	addComponent(&fpsCounter);
	addComponent(&renderDebug);
	addComponent(&cursor);
	addComponent(&inventoryHotbar);

	playerInventory = new SlotComponent*[9];
	for (int i = 0; i < 9; i++)
	{
		playerInventory[i] = new SlotComponent(*this, i, Game::getInstance().getPlayer()->getInventory()[i]);
		addComponent(playerInventory[i]);
	}

	addComponent(&inventorySelected);
}

GuiIngame::~GuiIngame()
{
	for (int i = 0; i < 9; i++)
	{
		delete(playerInventory[i]);
	}
	delete(playerInventory);
}

void GuiIngame::prepareLayout()
{
	// Set positions
	fpsCounter.setPosition(4, 12);
	renderDebug.setPosition(4, 32);
	cursor.setPosition(width / 2.0F - 8, height / 2.0F - 8);
	inventoryHotbar.setPosition(width / 2.0F - (182 / 2.0F), 0.0F);
	inventorySelected.setPosition(width / 2.0F - (24 / 2.0F), -1.0F);

	for (int i = 0; i < 9; i++)
	{
		playerInventory[i]->setPosition(width / 2.0F - (182 / 2.0F) + 20 * i + 3, 3.0F);
	}
}

void GuiIngame::render()
{
	Gui::render();

	for (int i = 0; i < 9; i++)
	{
		if (playerInventory[i]->slotBlock != Game::getInstance().getPlayer()->getInventory()[i])
		{
			playerInventory[i]->slotBlock = Game::getInstance().getPlayer()->getInventory()[i];
			playerInventory[i]->init();
		}
	}

	inventorySelected.setPosition(width / 2.0F - (182 / 2.0F) + 20 * Game::getInstance().getPlayer()->getCursor() - 1, -1.0F);

	if (TimeManager::fps == 0)
	{
		fpsCounter.setText("FPS: " + std::to_string(TimeManager::lastFps));
	}

	renderDebug.setText("G: " + std::to_string(Game::getInstance().getWorld()->getChunkGeneratorQueue().getInputSize()) + " R: " + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getChunkRenderQueue().getInputSize()) + "\n" +
	"(" + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getDrawnedChunk()) + "/" + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getChunkAmount()) + ")");
}
