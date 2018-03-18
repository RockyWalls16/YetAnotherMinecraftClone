#include "util/Color.h"

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{}

Color::Color(int r, int g, int b, int a) : r(r / 255.0F), g(g / 255.0F), b(b / 255.0F), a(a / 255.0F)
{}

Color Color::lerp(const Color & color, float t)
{
	return Color(r + (color.r - r) * t, g + (color.g - g) * t, b + (color.b - b) * t, a + (color.a - a) * t);
}
