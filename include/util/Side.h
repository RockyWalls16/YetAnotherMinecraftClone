#ifndef _UTIL_SIDE_H_
#define _UTIL_SIDE_H_

enum Side
{
	TOP,      // Y+
	BOTTOM,   // Y-
	EAST,     // X+
	NORTH,    // Z-
	WEST,     // X-
	SOUTH     // Z+
};

class SideUtil
{
public:
	static Side opposite[6];
	static int xValue[6];
	static int yValue[6];
	static int zValue[6];
};

#endif