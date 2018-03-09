#include <client/render/light/Light.h>

Light::Light(Color color, float ambiantAmount) : dirty(true), lightColor(color), ambiantAmount(ambiantAmount)
{
}

void Light::setColor(Color color)
{
	lightColor = color;
	dirty = true;
}

void Light::setAmbientAmount(float amount)
{
	ambiantAmount = amount;
	dirty = true;
}
