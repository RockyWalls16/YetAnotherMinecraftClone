#include <client/render/font/FontVAO.h>
#include <client/shaders/ShaderCache.h>


FontVAO::FontVAO(Font& font, VertexArray * fontVao, int vertexAmount, int width, int height) : font(font), fontVao(fontVao), vertexAmount(vertexAmount), width(width), height(height)
{

}

FontVAO::~FontVAO()
{
	delete(fontVao);
}

void FontVAO::render2D(int x, int y)
{
	fontVao->setIdentity();
	fontVao->translate(glm::vec3(x, y, 0));

	font.bind();
	ShaderCache::shader2d->use();
	fontVao->drawEBO(vertexAmount);
}
