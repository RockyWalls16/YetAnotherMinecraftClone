#ifndef CLIENT_GUI_BUTTONCOMPONENT_H_
#define CLIENT_GUI_BUTTONCOMPONENT_H_

#include <client/gui/components/TextureComponent.h>
#include <client/gui/components/TextComponent.h>
#include <string>

class ButtonComponent : public GuiComponent
{
private:
	TextComponent textComponent;
	TextureComponent buttonBegin;
	TextureComponent buttonEnd;
	int width;

public:
	ButtonComponent(const std::string & text, int width, Font& font);
	ButtonComponent(const std::string& text, int width);

	virtual void init() override;
	virtual void render() override;
	virtual void setPosition(int x, int y) override;

	virtual void onInput(int mX, int mY) override;
};

#endif