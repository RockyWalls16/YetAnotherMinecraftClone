#ifndef CLIENT_GUI_GUIINGAME_H_
#define CLIENT_GUI_GUIINGAME_H_

#include <client/gui/Gui.h>
#include <client/gui/components/TextComponent.h>
#include <client/gui/components/TextureComponent.h>
#include <client/gui/components/SlotComponent.h>

class GuiIngame : public Gui
{
private:
	TextComponent fpsCounter;
	TextComponent renderDebug;
	TextureComponent cursor;
	TextureComponent inventoryHotbar;
	TextureComponent inventorySelected;
	SlotComponent** playerInventory;

public:
	GuiIngame();
	~GuiIngame();

	virtual void prepareLayout() override;
	virtual void render() override;
	virtual bool blockInput() override { return false; }
	virtual bool closeWithEscape() { return false; }
};

#endif