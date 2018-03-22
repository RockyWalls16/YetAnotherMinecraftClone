#include <client/gui/Gui.h>
#include <util/Logger.h>
#include <client/render/GameRenderer.h>
#include <client/render/WindowManager.h>
#include <client/textures/TextureLoader.h>
#include <util/GameSettings.h>
#include <algorithm>

Texture* Gui::widgetsTexture = nullptr;

Gui::Gui() : initialized(false)
{
	
}

Gui::~Gui()
{
	for (GuiComponent* component : components)
	{
		delete(component);
	}
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
	int width;
	int height;
	WindowManager::getMainInstance().getFramebufferSize(&width, &height);
	onResize(width / GUI_SCALE, height / GUI_SCALE);

	prepareLayout(!initialized);
	GameRenderer::getInstance().getOpenGuis().push_back(this);
}

void Gui::close()
{
	std::vector<Gui*> guis = GameRenderer::getInstance().getOpenGuis();
	guis.erase(std::remove(guis.begin(), guis.end(), this), guis.end());
}

void Gui::initTextures()
{
	Info("LOADING");
	widgetsTexture = TextureLoader::loadTexture("Gui/widgets");
}
