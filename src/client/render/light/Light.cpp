#include <client/render/light/Light.h>

Light::Light(Color color) : dirty(true), lightColor(color)
{
}

void Light::setColor(Color color)
{
	lightColor = color;
	dirty = true;
}
