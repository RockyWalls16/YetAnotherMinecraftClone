#ifndef CLIENT_RENDER_LIGHT_LIGHT_H_
#define CLIENT_RENDER_LIGHT_LIGHT_H_

#include <util/Color.h>

class Light
{
public:
	bool dirty;
	Color lightColor;

	Light(Color color);

	void setColor(Color color);
};

#endif