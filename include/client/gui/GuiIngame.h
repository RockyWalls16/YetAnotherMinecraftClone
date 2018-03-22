#ifndef CLIENT_GUI_GUIINGAME_H_
#define CLIENT_GUI_GUIINGAME_H_

#include <client/gui/Gui.h>
#include <client/gui/components/TextComponent.h>
#include <client/gui/components/TextureComponent.h>

class GuiIngame : public Gui
{
private:
	TextComponent* fpsCounter;
	TextComponent* renderDebug;
	TextureComponent* cursor;
	TextureComponent* inventoryHotbar;
	TextureComponent* inventorySelected;

public:
	virtual void prepareLayout(bool onOpen) override;

	virtual void render() override;
};

#endif