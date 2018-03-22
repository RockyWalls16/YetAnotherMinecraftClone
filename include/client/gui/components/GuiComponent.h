#ifndef CLIENT_GUI_GUICOMPONENT_H_
#define CLIENT_GUI_GUICOMPONENT_H_

class GuiComponent
{
protected:
	int x;
	int y;

public:
	virtual void init() = 0;
	virtual void render() = 0;

	void setPosition(int x, int y);
};

#endif