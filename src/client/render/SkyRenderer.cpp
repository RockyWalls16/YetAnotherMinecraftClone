#include <client/render/SkyRenderer.h>
#include <client/shaders/ShaderCache.h>
#include <client/render/WorldRenderer.h>
#include <client/render/util/VertexBuilder.h>
#include <client/render/util/VertexArray.h>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <util/Logger.h>
#include <client/render/Camera.h>
#include <client/render/GameRenderer.h>
#include <math/MathUtil.h>
#include <client/textures/TextureLoader.h>

#define STARS_AMOUNT 20000


// Sundown start at 19H00 -> fade to orange
#define SUN_DOWN_START 0.8
#define SKY_SUN_DOWN_TOP Color(0.1F, 0.45F, 0.7F)
#define SKY_SUN_DOWN_BOTTOM Color(1.0F, 0.6F, 0.0F)

// Sundown end at 20H00 -> fade to black
#define SUN_DOWN_END 0.85

// Total Night at 20H30 -> stay black
#define NIGHT 0.9
#define SKY_NIGHT_TOP Color(0.0F, 0.0F, 0.02F)
#define SKY_NIGHT_BOTTOM Color(0.01F, 0.02F, 0.05F)

// Sunrise start at 7H00 -> fade to orange
#define SUNRISE_START 0.3

// Sunrise end at 7H30 -> fade to blue
#define SUNRISE_END 0.35

// Day at 08H00 -> stay blue
#define DAY_START 0.4
#define SKY_DAY_TOP Color(0.5F, 0.65F, 1.0F)
#define SKY_DAY_BOTTOM Color(0.71F, 0.81F, 1.0F)

SkyRenderer::SkyRenderer(WorldRenderer* worldRenderer) : worldRenderer(worldRenderer), sunLight(Color(1.0F, 0.0F, 1.0F), glm::vec3(-0.75F, -1.0F, -0.35F))
{
	starTexture = TextureLoader::loadTexture("star");
	createSphere(6, 6);
	createStarField();
}

void SkyRenderer::render()
{
	float dayPercent = worldRenderer->getWorld()->getDayPercent();

	ShaderCache::skyShader->use();

	// Set sky color
	float starAlpha = 0.0F;
	Color topColor = SKY_DAY_TOP;
	Color bottomColor = SKY_DAY_BOTTOM;

	// Sundown transition (blue->orange)
	if (dayPercent >= SUN_DOWN_START && dayPercent < SUN_DOWN_END)
	{
		float percent = (dayPercent - SUN_DOWN_START) / (SUN_DOWN_END - SUN_DOWN_START);
		topColor = SKY_DAY_TOP.lerp(SKY_SUN_DOWN_TOP, percent);
		bottomColor = SKY_DAY_BOTTOM.lerp(SKY_SUN_DOWN_BOTTOM, percent);
	}
	// Sundown transition (orange->black)
	else if (dayPercent >= SUN_DOWN_END && dayPercent < NIGHT)
	{
		float percent = (dayPercent - SUN_DOWN_END) / (NIGHT - SUN_DOWN_END);
		starAlpha = MathUtil::lerp(0.0F, 1.0F, percent);
		topColor = SKY_SUN_DOWN_TOP.lerp(SKY_NIGHT_TOP, percent);
		bottomColor = SKY_SUN_DOWN_BOTTOM.lerp(SKY_NIGHT_BOTTOM, percent);
	}
	// Night still (black)
	else if (dayPercent >= NIGHT || dayPercent < SUNRISE_START)
	{
		starAlpha = 1.0F;
		topColor = SKY_NIGHT_TOP;
		bottomColor = SKY_NIGHT_BOTTOM;
	}
	// Sunrise transition (black->orange)
	else if (dayPercent >= SUNRISE_START && dayPercent < SUNRISE_END)
	{
		float percent = (dayPercent - SUNRISE_START) / (SUNRISE_END - SUNRISE_START);
		starAlpha = MathUtil::lerp(1.0F, 0.0F, percent);
		topColor = SKY_NIGHT_TOP.lerp(SKY_SUN_DOWN_TOP, percent);
		bottomColor = SKY_NIGHT_BOTTOM.lerp(SKY_SUN_DOWN_BOTTOM, percent);
	}
	// Sunrise transition (orange->blue)
	else if (dayPercent >= SUNRISE_END && dayPercent < DAY_START)
	{
		float percent = (dayPercent - SUNRISE_END) / (DAY_START - SUNRISE_END);
		topColor = SKY_SUN_DOWN_TOP.lerp(SKY_DAY_TOP, percent);
		bottomColor = SKY_SUN_DOWN_BOTTOM.lerp(SKY_DAY_BOTTOM, percent);
	}

	ShaderCache::skyShader->setColors(topColor, bottomColor);
	skySphere->setTranslate(GameRenderer::getInstance().getGameCamera()->getLocation());
	skySphere->drawVAO(skySphereVertexAmount, 0, GL_TRIANGLE_STRIP);
	 
	glEnable(GL_BLEND);

	starTexture->bind();
	ShaderCache::starShader->use();
	ShaderCache::starShader->setStarAlpha(starAlpha);
	starField->setIdentity();
	starField->translate(GameRenderer::getInstance().getGameCamera()->getLocation());
	starField->rotate(glm::radians((worldRenderer->getWorld()->getTime() / (float) DAY_DURATION) * 20.5), glm::vec3(0.0F, 0.33F, 1.0F));
	starField->drawVAO(STARS_AMOUNT, 0, GL_POINTS);

	glDisable(GL_BLEND);
}

void SkyRenderer::createSphere(int lats, int longs)
{
	int vertexAmount = (lats + 1) * (longs + 1) * 6;
	float* vertices = new float[vertexAmount];

	int currentVertex = 0;
	for (int i = 0; i <= lats; i++)
	{
		float lat0 = M_PI * (-0.5F + (float)(i - 1) / lats);
		float z0 = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = M_PI * (-0.5F + (float)i / lats);
		float z1 = sin(lat1);
		float zr1 = cos(lat1);

		for (int j = 0; j <= longs; j++)
		{
			float lng = 2 * M_PI * (float)(j - 1) / longs;
			float x = cos(lng);
			float y = sin(lng);

			vertices[currentVertex] = x * zr0;
			vertices[currentVertex + 1] = y * zr0;
			vertices[currentVertex + 2] = z0;

			vertices[currentVertex + 3] = x * zr1;
			vertices[currentVertex + 4] = y * zr1;
			vertices[currentVertex + 5] = z1;

			currentVertex += 6;
		}
	}

	skySphere = VertexArray::makeVAO();
	skySphere->disableNormals();
	skySphere->addVBO(vertices, vertexAmount * sizeof(float), GL_STATIC_DRAW);
	skySphere->assignPositionAttrib(0, 0, sizeof(float) * 3);
	skySphere->scale(glm::vec3(FAR_PLANE - 200, FAR_PLANE - 200, FAR_PLANE - 200));
	
	skySphereVertexAmount = vertexAmount / 3;
}

void SkyRenderer::createStarField()
{
	float starsVertices[STARS_AMOUNT * 5];
	for (int i = 0; i < STARS_AMOUNT; i+=5)
	{
		float theta = 2 * M_PI * MathUtil::randF();
		float phi = 2 * MathUtil::randF() - 1;
		float sqrtPhi = sqrt(1 - phi * phi);
		starsVertices[i] = sqrtPhi * cos(theta);
		starsVertices[i + 2] = sqrtPhi * sin(theta);
		starsVertices[i + 1] = phi;

		starsVertices[i + 3] = MathUtil::randF(0.2F, 0.9F); // Star shininess
		starsVertices[i + 4] = MathUtil::randF(2.5F, 3.5F); // Star size
	}

	starField = VertexArray::makeVAO();
	starField->addVBO(starsVertices, STARS_AMOUNT * 5 * sizeof(float), GL_STATIC_DRAW);
	starField->assignPositionAttrib(0, 0, 5 * sizeof(float));
	starField->assignVertexAttrib(0, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*) (3 * sizeof(float)));
}

DirectionalLight & SkyRenderer::getSunLight()
{
	return sunLight;
}
