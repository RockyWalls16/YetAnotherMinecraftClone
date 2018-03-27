#include <client/gui/GuiPause.h>
#include <client/gui/GuiSettings.h>
#include <client/input/GameController.h>
#include <util/Logger.h>
#include <Game.h>

GuiPause::GuiPause() : 
	pauseTitle("OGL-VS", CENTER),
	resumeButton(*this, "Resume", 200),
	settingsButton(*this, "Settings", 200),
	quitButton(*this, "Quit", 200)
{
	addComponent(&pauseTitle);
	addComponent(&resumeButton);
	addComponent(&settingsButton);
	addComponent(&quitButton);
}

void GuiPause::prepareLayout()
{
	pauseTitle.setPosition(width / 2, height / 2 + 50);
	resumeButton.setPosition(width / 2 - 100, height / 2 + 10);
	settingsButton.setPosition(width / 2 - 100, height / 2 - 15);
	quitButton.setPosition(width / 2 - 100, height / 2 - 40);
}

void GuiPause::onInputEvent(GuiComponent * component)
{
	if (&resumeButton == component)
	{
		close();
	}

	if (&settingsButton == component)
	{
		GuiSettings* settings = new GuiSettings();
		settings->open();

		close();
	}

	if (&quitButton == component)
	{
		Game::getInstance().closeGame();
	}
}