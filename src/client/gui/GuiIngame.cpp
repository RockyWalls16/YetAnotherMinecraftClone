#include <client/gui/GuiIngame.h>
#include <util/TimeManager.h>
#include <Game.h>
#include <client/render/GameRenderer.h>
#include <core/world/World.h>
#include <client/textures/TextureCache.h>

void GuiIngame::prepareLayout(bool onOpen)
{
	if (onOpen)
	{
		fpsCounter = new TextComponent("...");
		renderDebug = new TextComponent("...");
		cursor = new TextureComponent(Gui::widgetsTexture, 16, 16, 240.0F / 256.0F, 240.0F / 256.0F, 1.0F, 1.0F);
		inventoryHotbar = new TextureComponent(Gui::widgetsTexture, 182, 22, 0.0F, 234.0F / 256.0F, 182.0F / 256.0F, 1.0F);
		inventorySelected = new TextureComponent(Gui::widgetsTexture, 24, 24, 0.0F, 210.0F / 256.0F, 24.0F / 256.0F, 234.0F / 256.0F);
		addComponent(fpsCounter);
		addComponent(renderDebug);
		addComponent(cursor);
		addComponent(inventoryHotbar);
		addComponent(inventorySelected);
	}

	fpsCounter->setPosition(4, 0);
	renderDebug->setPosition(4, 24);
	cursor->setPosition(width / 2.0F - 8, height / 2.0F - 8);
	inventoryHotbar->setPosition(width / 2.0F - (182 / 2.0F), 0.0F);
	inventorySelected->setPosition(width / 2.0F - (24 / 2.0F), -1.0F);
}

void GuiIngame::render()
{
	Gui::render();

	if (TimeManager::fps == 0)
	{
		fpsCounter->setText("FPS: " + std::to_string(TimeManager::lastFps));

	}

	renderDebug->setText("G: " + std::to_string(Game::getInstance().getWorld()->getChunkGeneratorQueue().getInputSize()) + " R: " + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getChunkRenderQueue().getInputSize()) + "\n" +
	"(" + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getDrawnedChunk()) + "/" + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getChunkAmount()) + ")");
}
