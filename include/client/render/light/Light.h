#ifndef CLIENT_RENDER_LIGHT_LIGHT_H_
#define CLIENT_RENDER_LIGHT_LIGHT_H_

#include <util/Color.h>

class Light
{
public:
	Color lightColor;

	Light(Color color);
};

#endif