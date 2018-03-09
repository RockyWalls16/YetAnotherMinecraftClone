#ifndef CLIENT_RENDER_LIGHT_LIGHT_H_
#define CLIENT_RENDER_LIGHT_LIGHT_H_

#include <util/Color.h>

class Light
{
public:
	bool dirty;
	Color lightColor;
	float ambiantAmount;

	Light(Color color, float ambiantAmount);

	void setColor(Color color);

	void setAmbientAmount(float amount);
};

#endif