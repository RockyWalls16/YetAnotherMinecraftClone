#include "util/Color.h"

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{}

Color::Color(int r, int g, int b, int a) : r(r * 255), g(g * 255), b(b * 255), a(a * 255)
{}

Color Color::lerp(const Color & color, float t)
{
	return Color(r + (color.r - r) * t, g + (color.g - g) * t, b + (color.b - b) * t, a + (color.a - a) * t);
}
