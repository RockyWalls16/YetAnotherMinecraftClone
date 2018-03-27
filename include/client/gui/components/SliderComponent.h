#ifndef CLIENT_GUI_SLIDERCOMPONENT_H_
#define CLIENT_GUI_SLIDERCOMPONENT_H_

#include <client/gui/components/TextureComponent.h>
#include <client/gui/components/TextComponent.h>
#include <string>

class Gui;

class SliderComponent : public GuiComponent
{
private:
	Gui & parent;
	TextComponent textComponent;
	TextureComponent backgroundBegin;
	TextureComponent backgroundEnd;
	TextureComponent cursorBegin;
	TextureComponent cursorEnd;
	int width;
	float value;
	float min;
	float max;
	bool grabbed;

public:
	SliderComponent(Gui& parent, const std::string & text, int width, Font& font);
	SliderComponent(Gui& parent, const std::string& text, int width);

	virtual void init() override;
	virtual void render() override;
	virtual void setPosition(int x, int y) override;
	virtual void onInput(int mX, int mY) override;

	float getValue();
	void setValue(float newValue);
	void setBounds(float min, float max);
	void setText(const std::string& newText);
};

#endif