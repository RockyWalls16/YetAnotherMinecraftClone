#ifndef CLIENT_GUI_GUISETTINGS_H_
#define CLIENT_GUI_GUISETTINGS_H_

#include <client/gui/Gui.h>
#include <client/gui/components/TextComponent.h>
#include <client/gui/components/ButtonComponent.h>
#include <client/gui/components/SliderComponent.h>

class GuiSettings : public Gui
{
private:
	TextComponent settingsTitle;
	SliderComponent fovSlider;
	SliderComponent renderDistanceSlider;
	SliderComponent volumeSlider;
	ButtonComponent backButton;

public:
	GuiSettings();

	virtual void prepareLayout() override;

	virtual void onInputEvent(GuiComponent* component);
};

#endif