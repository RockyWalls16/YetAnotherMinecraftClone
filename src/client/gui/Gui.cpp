#include <client/gui/Gui.h>
#include <util/Logger.h>
#include <client/input/GameController.h>
#include <client/render/GameRenderer.h>
#include <client/render/WindowManager.h>
#include <client/textures/TextureLoader.h>
#include <util/GameSettings.h>
#include <algorithm>

Texture* Gui::widgetsTexture = nullptr;

Gui::Gui() : initialized(false)
{
	
}

void Gui::addComponent(GuiComponent* component)
{
	components.push_back(component);
	component->init();
}

void Gui::render()
{
	for (GuiComponent* component : components)
	{
		component->render();
	}
}

void Gui::onResize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Gui::open()
{
	shallClose = false;

	int width;
	int height;
	WindowManager::getMainInstance().getFramebufferSize(&width, &height);
	onResize(width / GUI_SCALE, height / GUI_SCALE);

	prepareLayout();
	GameRenderer::getInstance().getOpenGuis().push_back(this);
}

void Gui::close()
{
	shallClose = true;
}

bool Gui::shallUiClose()
{
	return shallClose;
}

void Gui::onInputUpdate(int mX, int mY)
{
	for (GuiComponent* component : components)
	{
		component->onInput(mX, mY);
	}

	if (closeWithEscape() && GameController::ESCAPE_KEY->isPressed())
	{
		close();
	}
}

void Gui::initTextures()
{
	widgetsTexture = TextureLoader::loadTexture("Gui/widgets");
}
