#include <util/Side.h>

Side SideUtil::opposite[6] = { Side::BOTTOM, Side::TOP, Side::WEST, Side::SOUTH, Side::EAST, Side::NORTH };
int SideUtil::xValue[6] = { 0, 0, 1, 0, -1, 0 };
int SideUtil::yValue[6] = { 1, -1, 0, 0, 0, 0 };
int SideUtil::zValue[6] = { 0, 0, 0, -1, 0, 1 };