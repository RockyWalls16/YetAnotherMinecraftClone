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
	addComponent(&inventorySelected);
}

void GuiIngame::prepareLayout()
{
	// Set positions
	fpsCounter.setPosition(4, 0);
	renderDebug.setPosition(4, 24);
	cursor.setPosition(width / 2.0F - 8, height / 2.0F - 8);
	inventoryHotbar.setPosition(width / 2.0F - (182 / 2.0F), 0.0F);
	inventorySelected.setPosition(width / 2.0F - (24 / 2.0F), -1.0F);
}

void GuiIngame::render()
{
	Gui::render();

	if (TimeManager::fps == 0)
	{
		fpsCounter.setText("FPS: " + std::to_string(TimeManager::lastFps));
	}

	renderDebug.setText("G: " + std::to_string(Game::getInstance().getWorld()->getChunkGeneratorQueue().getInputSize()) + " R: " + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getChunkRenderQueue().getInputSize()) + "\n" +
	"(" + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getDrawnedChunk()) + "/" + std::to_string(GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().getChunkAmount()) + ")");
}
