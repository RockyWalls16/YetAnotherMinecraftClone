#ifndef CLIENT_GUI_GUIPAUSE_H_
#define CLIENT_GUI_GUIPAUSE_H_

#include <client/gui/Gui.h>
#include <client/gui/components/TextComponent.h>
#include <client/gui/components/ButtonComponent.h>

class GuiPause : public Gui
{
private:
	TextComponent pauseTitle;
	ButtonComponent resumeButton;
	ButtonComponent settingsButton;
	ButtonComponent quitButton;

public:
	GuiPause();

	virtual void prepareLayout() override;

	virtual void onInputEvent(GuiComponent* component);
};

#endif