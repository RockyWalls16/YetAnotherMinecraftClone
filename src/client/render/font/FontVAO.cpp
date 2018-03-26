#include <client/render/font/FontVAO.h>
#include <client/shaders/ShaderCache.h>
#include <util/Logger.h>

FontVAO::FontVAO(Font& font) : font(font), vertexAmount(0)
{
	fontVao.disableNormals();
	fontVao.addVBO(NULL, 0, GL_DYNAMIC_DRAW);
	fontVao.enableEBO(NULL, 0, GL_DYNAMIC_DRAW);
	fontVao.assignPositionAttrib(0, 0, sizeof(float) * 9);
	fontVao.assignRGBAAttrib(0, 1, sizeof(float) * 9, (void*)(3 * sizeof(float)));
	fontVao.assignUVAttrib(0, 2, sizeof(float) * 9, (void*)(7 * sizeof(float)));
}

FontVAO::~FontVAO()
{
	
}

void FontVAO::updateVAO(VertexBuilder& vb, int width, int height)
{
	fontVao.updateVBO(0, vb.getVertexBuffer(), vb.getVBOSize(), GL_DYNAMIC_DRAW);
	fontVao.updateEBO(vb.getIndicesBuffer(), vb.getEOBSize(), GL_DYNAMIC_DRAW);

	vertexAmount = vb.getEOBSize();
	this->width = width;
	this->height = height;
}

void FontVAO::render2D(int x, int y)
{
	if (vertexAmount > 0)
	{
		fontVao.setIdentity();
		fontVao.translate(glm::vec3(x, y, 0));

		font.bind();
		ShaderCache::shader2d->use();
		fontVao.drawEBO(vertexAmount);
	}
}

int FontVAO::getWidth()
{
	return width;
}

int FontVAO::getHeight()
{
	return height;
}
