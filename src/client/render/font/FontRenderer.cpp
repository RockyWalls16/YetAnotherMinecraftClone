#include <client/render/font/FontRenderer.h>
#include <client/render/font/FontVAO.h>
#include <client/render/util/VertexBuilder.h>
#include <client/render/util/VertexArray.h>
#include <util/Logger.h>

void FontRenderer::makeVao(FontVAO& fontVao, Font& font, const std::string& text)
{
	static unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	static float vertices[] =
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

	static VertexBuilder vb(9, 1000);
	vb.rewind();

	int charAmount = text.size();
	const char* charaters = text.c_str();
	CharInfo* chInfo;

	int currentWidth = 0;
	int currentHeight = 0;
	int totalWidth = 0;
	int totalHeight = 0;
	int currentMaxHeight = 0;

	// Build vertex array
	for (int i = 0; i < charAmount; i++)
	{
		char c = charaters[i];

		// Handle space
		if (c == ' ')
		{
			currentWidth += 12;
			continue;
		}
		// Handle new line
		else if (c == '\n')
		{
			currentHeight -= font.getLineHeight();
			totalHeight += font.getLineHeight();

			if (currentWidth > totalWidth)
			{
				totalWidth = currentWidth;
			}

			currentWidth = 0;
			continue;
		}

		// Get char info
		chInfo = font.getCharInfo(c);
		if (chInfo == nullptr)
		{
			continue;
		}

		// Calculate UV & coord
		int width = chInfo->x2 - chInfo->x;
		int height = chInfo->y2 - chInfo->y;

		float u, v, u2, v2;
		font.getUV(chInfo, &u, &v, &u2, &v2);

		int yPos = - totalHeight - font.getBaseLine();
		int yOffset = (font.getBaseLine() - height) - chInfo->offsetY;

		// Prepare vertices
		float charVertices[36];
		for (int j = 0; j < 36; j += 9)
		{
			// Positions
			charVertices[j] = vertices[j] * width + currentWidth + chInfo->offsetX;
			charVertices[j + 1] = vertices[j + 1] * height + yPos + yOffset;
			charVertices[j + 2] = vertices[j + 2];

			// Color
			charVertices[j + 3] = vertices[j + 3];
			charVertices[j + 4] = vertices[j + 4];
			charVertices[j + 5] = vertices[j + 5];
			charVertices[j + 6] = vertices[j + 6];

			// UV
			charVertices[j + 7] = vertices[j + 7] * u2 + u;
			charVertices[j + 8] = vertices[j + 8] * v2 + v;
		}

		// Indices
		unsigned int charIndices[6];
		unsigned int lastIndice = vb.getLastIndice();
		for (int j = 0; j < 6; j++)
		{
			charIndices[j] = indices[j] + lastIndice;
		}

		// Update vertex builder
		vb.setLastIndice(lastIndice + 4);
		vb.addVertices(charVertices, 4, charIndices, 6);

		// Update offset
		currentWidth += chInfo->xAdvance;
		if (height > currentMaxHeight)
		{
			currentHeight = height;
		}
	}

	// Calculate text bounds
	if (currentWidth > totalWidth)
	{
		totalWidth = currentWidth;
	}
	totalHeight += currentHeight;

	// Generate Vertex data
	fontVao.updateVAO(vb, totalWidth, totalHeight);
}
