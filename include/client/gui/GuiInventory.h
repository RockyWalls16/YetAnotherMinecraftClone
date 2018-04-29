#ifndef CLIENT_GUI_GUIINVENTORY_H_
#define CLIENT_GUI_GUIINVENTORY_H_

#include <client/gui/Gui.h>
#include <client/gui/components/TextComponent.h>
#include <client/gui/components/TextureComponent.h>
#include <client/gui/components/CreativeSlotComponent.h>

class GuiInventory : public Gui
{
public:
	static Texture* inventoryTexture;

private:
	TextComponent inventoryTitle;
	TextureComponent inventoryBackground;
	SlotComponent** creativeSlots;
	SlotComponent** playerInventory;

public:
	GuiInventory();
	~GuiInventory();

	virtual void prepareLayout() override;

	virtual void onInputEvent(GuiComponent* component) override;
	virtual void onInputUpdate(int mX, int mY) override;
	virtual void onSlotHoverChange(SlotComponent& slot) override;
	virtual void onSlotChange(SlotComponent& slot) override;
};

#endif