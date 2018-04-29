#ifndef CLIENT_GUI_GUI_H_
#define CLIENT_GUI_GUI_H_

#include <vector>
#include <client/gui/components/GuiComponent.h>
#include <client/textures/Texture.h>

class SlotComponent;

class Gui
{
public:
	static Texture* widgetsTexture;

private:
	std::vector<GuiComponent*> components;
	bool initialized;
	bool shallClose;

protected:
	int width;
	int height;

public:
	Gui();

	virtual void prepareLayout() = 0;
	void addComponent(GuiComponent* component);
	void removeComponent(GuiComponent* component);
	virtual void render();
	virtual void onResize(int width, int height);
	virtual void onInputEvent(GuiComponent* component) {}
	virtual bool blockInput() { return true; }
	virtual bool closeWithEscape() { return true; }

	virtual void onSlotHoverChange(SlotComponent& slot) {}
	virtual void onSlotChange(SlotComponent& slot) {}

	void open();
	void close();
	bool shallUiClose();
	virtual void onInputUpdate(int mX, int mY);

	static void initTextures();
};

#endif