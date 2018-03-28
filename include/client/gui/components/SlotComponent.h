#ifndef CLIENT_GUI_SLOTCOMPONENT_H_
#define CLIENT_GUI_SLOTCOMPONENT_H_

#include <client/gui/components/TextureComponent.h>
#include <client/gui/components/TextComponent.h>
#include <string>
#include <core/block/Block.h>

class Gui;

class SlotComponent : public GuiComponent
{
private:
	Gui & parent;
	Block* block;
	bool hovered;

public:
	SlotComponent(Gui& parent, Block* block);

	virtual void init() override;
	virtual void render() override;
	virtual void setPosition(int x, int y) override;
	virtual void onInput(int mX, int mY) override;
	bool isHovered();
	Block* getBlock();
};

#endif