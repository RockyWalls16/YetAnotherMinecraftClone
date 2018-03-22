#ifndef CLIENT_GUI_TEXTCOMPONENT_H_
#define CLIENT_GUI_TEXTCOMPONENT_H_

#include <client/gui/components/GuiComponent.h>
#include <client/render/font/Font.h>
#include <client/render/font/FontVAO.h>
#include <string>

class TextComponent : public GuiComponent
{
private:
	Font& font;
	FontVAO fontVao;
	std::string text;

public:
	TextComponent(const std::string& text, Font& font);
	TextComponent(const std::string& text);

	virtual void init() override;
	virtual void render() override;
	void setText(const std::string& newText);
};

#endif