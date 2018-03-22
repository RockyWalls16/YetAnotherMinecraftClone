#ifndef CLIENT_GUI_TEXTURECOMPONENT_H_
#define CLIENT_GUI_TEXTURECOMPONENT_H_

#include <client/gui/components/GuiComponent.h>
#include <client/textures/Texture.h>
#include <client/render/util/VertexArray.h>

class TextureComponent : public GuiComponent
{
private:
	VertexArray vao;
	Texture* texture;
	int width;
	int height;
	float sX;
	float sY;
	float sX2;
	float sY2;

public:
	TextureComponent(Texture* texture, float w, float h, float sX, float sY, float sX2, float sY2);

	virtual void init() override;
	virtual void render() override;
};

#endif