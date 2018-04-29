#ifndef CLIENT_GUI_CREATIVESLOTCOMPONENT_H_
#define CLIENT_GUI_CREATIVESLOTCOMPONENT_H_

#include <client/gui/components/SlotComponent.h>

class CreativeSlotComponent : public SlotComponent
{
public:
	CreativeSlotComponent(Gui& parent, int id, Block* block) : SlotComponent(parent, id, block) {}

	// Needs check on parent -> crash
	/*virtual void Pick() override
	{
		draggedSlot = new SlotComponent(*this);
		parent.addComponent(draggedSlot);
	}*/

	/*virtual void Swap() override
	{
		draggedSlot->parent.removeComponent(draggedSlot);
		delete(draggedSlot);
	}*/
};

#endif