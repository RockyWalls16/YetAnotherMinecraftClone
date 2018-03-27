#include <client/gui/GuiPause.h>
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
	pauseTitle.setPosition(width / 2, height - height / 4);
	resumeButton.setPosition(width / 2 - 100, height - height / 4 - 40);
	settingsButton.setPosition(width / 2 - 100, height - height / 4 - 65);
	quitButton.setPosition(width / 2 - 100, height - height / 4 - 90);
}

void GuiPause::onInputEvent(GuiComponent * component)
{
	if (&resumeButton == component)
	{
		close();
	}

	if (&quitButton == component)
	{
		Game::getInstance().closeGame();
	}
}