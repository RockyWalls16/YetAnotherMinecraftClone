#ifndef CLIENT_GUI_GUI_H_
#define CLIENT_GUI_GUI_H_

#include <vector>
#include <client/gui/components/GuiComponent.h>
#include <client/textures/Texture.h>

class Gui
{
public:
	static Texture* widgetsTexture;

private:
	std::vector<GuiComponent*> components;
	bool initialized;

protected:
	int width;
	int height;

public:
	Gui();
	~Gui();

	virtual void prepareLayout(bool onOpen) = 0;
	void addComponent(GuiComponent* component);
	virtual void render();
	virtual void onResize(int width, int height);
	void open();
	void close();
	void onInput(int mX, int mY);

	static void initTextures();
};

#endif