#ifndef CLIENT_GUI_SLOTCOMPONENT_H_
#define CLIENT_GUI_SLOTCOMPONENT_H_

#include <client/gui/components/TextureComponent.h>
#include <client/gui/components/TextComponent.h>
#include <string>
#include <core/block/Block.h>

class Gui;

class SlotComponent : public GuiComponent
{
public:
	static SlotComponent* draggedSlot;
	Gui & parent;
	int id;
	TextureComponent slotTexture;
	Block* slotBlock;
	bool hovered;

public:
	SlotComponent(Gui& parent, int id, Block* block);
	SlotComponent(const SlotComponent& other);

	virtual void init() override;
	virtual void render() override;
	virtual void setPosition(int x, int y) override;
	virtual void onInput(int mX, int mY) override;
	bool isHovered();
	Block* getBlock();

	virtual void Pick();
	virtual void Swap();
	virtual void Place();
};

#endif