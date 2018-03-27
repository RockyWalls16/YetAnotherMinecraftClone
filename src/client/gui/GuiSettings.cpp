#include <client/gui/GuiSettings.h>
#include <client/gui/GuiPause.h>
#include <util/GameSettings.h>
#include <client/render/GameRenderer.h>

GuiSettings::GuiSettings() :
	settingsTitle("Settings", CENTER),
	fovSlider(*this, "Fov:...", 200),
	renderDistanceSlider(*this, "Render distance:...", 200),
	volumeSlider(*this, "Volume", 200),
	backButton(*this, "Back", 200)
{
	fovSlider.setBounds(30, 140);
	fovSlider.setValue(GameRenderer::getInstance().getGameCamera().getFov());
	renderDistanceSlider.setBounds(4, 16);
	renderDistanceSlider.setValue(GameSettings::renderDistance);
	volumeSlider.setValue(GameSettings::volume);
	addComponent(&settingsTitle);
	addComponent(&fovSlider);
	addComponent(&renderDistanceSlider);
	addComponent(&volumeSlider);
	addComponent(&backButton);
}

void GuiSettings::prepareLayout()
{
	settingsTitle.setPosition(width / 2, height / 2 + 50);
	fovSlider.setPosition(width / 2 - 100, height / 2 + 10);
	renderDistanceSlider.setPosition(width / 2 - 100, height / 2 - 15);
	volumeSlider.setPosition(width / 2 - 100, height / 2 - 40);
	backButton.setPosition(width / 2 - 100, height / 2 - 65);
}

void GuiSettings::onInputEvent(GuiComponent * component)
{
	if (component == &backButton)
	{
		GuiPause* pause = new GuiPause();
		pause->open();

		close();
	}

	if (component == &fovSlider)
	{
		GameRenderer::getInstance().setFov(fovSlider.getValue());
		fovSlider.setText("Fov: " + std::to_string((int) fovSlider.getValue()));
	}

	if (component == &renderDistanceSlider)
	{
		GameSettings::renderDistance = (int)renderDistanceSlider.getValue();
		renderDistanceSlider.setText("Render distance: " + std::to_string((int)renderDistanceSlider.getValue()));
	}

	if (component == &volumeSlider)
	{
		GameSettings::volume = volumeSlider.getValue();
		volumeSlider.setText("Volume: " + std::to_string((int)(volumeSlider.getValue() * 100.0F)) + "%");
	}
}