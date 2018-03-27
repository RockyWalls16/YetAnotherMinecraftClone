#ifndef CLIENT_GUI_TEXTCOMPONENT_H_
#define CLIENT_GUI_TEXTCOMPONENT_H_

#include <client/gui/components/GuiComponent.h>
#include <client/render/font/Font.h>
#include <client/render/font/FontVAO.h>
#include <string>

enum TextAlign
{
	LEFT,
	CENTER,
	RIGHT
};

class TextComponent : public GuiComponent
{
private:
	Font& font;
	FontVAO fontVao;
	std::string text;
	TextAlign horizontalAlign;

public:
	TextComponent(const std::string& text, TextAlign horizontalAlign, Font& font);
	TextComponent(const std::string& text, TextAlign horizontalAlign = LEFT);

	virtual void init() override;
	virtual void render() override;
	void setText(const std::string& newText);

	int getWidth();
	int getHeight();
};

#endif