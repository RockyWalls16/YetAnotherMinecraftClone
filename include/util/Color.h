#ifndef _UTIL_COLOR_H_
#define _UTIL_COLOR_H_

class Color
{
public:
	float r;
	float g;
	float b;
	float a;

	Color(float r, float g, float b, float a = 1.0);
	Color(int r, int g, int b, int a = 255);

	Color lerp(const Color& color, float t);
};

#endif