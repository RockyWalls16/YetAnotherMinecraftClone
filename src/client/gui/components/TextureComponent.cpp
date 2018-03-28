#include <client/gui/components/TextureComponent.h>
#include <client/shaders/ShaderCache.h>
#include <util/GLHeader.h>

TextureComponent::TextureComponent(Texture * texture, float w, float h, float sX, float sY, float sX2, float sY2) : texture(texture), width(w), height(h), sX(sX), sY(sY), sX2(sX2), sY2(sY2)
{
	vao.disableNormals();
	vao.addVBO(NULL, 0, GL_DYNAMIC_DRAW);
	vao.enableEBO(NULL, 0, GL_DYNAMIC_DRAW);
	vao.assignPositionAttrib(0, 0, sizeof(float) * 9);
	vao.assignRGBAAttrib(0, 1, sizeof(float) * 9, (void*)(3 * sizeof(float)));
	vao.assignUVAttrib(0, 2, sizeof(float) * 9, (void*)(7 * sizeof(float)));
}

TextureComponent::TextureComponent(Texture * texture, float w, float h, int sX, int sY, int sX2, int sY2, int texWidth, int texHeight) :
	TextureComponent(texture, w, h, sX / (float) texWidth, 1.0F - (sY2 / (float) texHeight), sX2 / (float) texWidth, 1.0F - (sY / (float)texHeight))
{
}

void TextureComponent::init()
{
	static float quadVertices[] =
	{
		0.0F, 1.0F, 0.0F,       //p
		1.0F, 1.0F, 1.0F, 1.0F, //c
		0.0F, 1.0F,             //uv

		0.0F, 0.0F, 0.0F,       //p
		1.0F, 1.0F, 1.0F, 1.0F, //c
		0.0F, 0.0F,             //uv

		1.0F, 1.0F, 0.0F,       //p
		1.0F, 1.0F, 1.0F, 1.0F, //c
		1.0F, 1.0F,             //uv

		1.0F, 0.0F, 0.0F,       //p
		1.0F, 1.0F, 1.0F, 1.0F, //c
		1.0F, 0.0F              //uv
	};

	float vertices[36];
	for (int i = 0; i < 36; i += 9)
	{
		// Positions
		vertices[i] = quadVertices[i] * width;
		vertices[i + 1] = quadVertices[i + 1] * height;
		vertices[i + 2] = quadVertices[i + 2];

		// Colors
		vertices[i + 3] = quadVertices[i + 3];
		vertices[i + 4] = quadVertices[i + 4];
		vertices[i + 5] = quadVertices[i + 5];
		vertices[i + 6] = quadVertices[i + 6];

		// UV
		vertices[i + 7] = quadVertices[i + 7] == 0.0F ? sX : sX2;
		vertices[i + 8] = quadVertices[i + 8] == 0.0F ? sY : sY2;
	}

	static unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	vao.updateVBO(0, vertices, 36 * sizeof(float), GL_DYNAMIC_DRAW);
	vao.updateEBO(indices, 6 * sizeof(unsigned int), GL_DYNAMIC_DRAW);
}

void TextureComponent::render()
{
	vao.setIdentity();
	vao.translate(glm::vec3(x, y, 0));

	texture->bind();
	ShaderCache::shader2d->use();
	vao.drawEBO(6);
}

void TextureComponent::setUV(float sX, float sY, float sX2, float sY2)
{
	this->sX = sX;
	this->sY = sY;
	this->sX2 = sX2;
	this->sY2 = sY2;

	init();
}

void TextureComponent::setUV(int sX, int sY, int sX2, int sY2, int texWidth, int texHeight)
{
	setUV(sX / (float)texWidth, 1.0F - (sY2 / (float)texHeight), sX2 / (float)texWidth, 1.0F - (sY / (float)texHeight));
}

void TextureComponent::setTexture(Texture * newTexture)
{
	texture = newTexture;
}
