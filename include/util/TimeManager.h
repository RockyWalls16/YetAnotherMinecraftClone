/*
 * TimeManager.h
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

#define TICK_PER_SECOND 20.0
#define TICK_DELAY (1.0 / TICK_PER_SECOND)
#define MAJOR_TICK_DELAY 4

#include <time.h>

class TimeManager
{
private:
	static double lastTime;
	static double secondProgress;
	static double tickInterpolation;
	static double tickTime;
	static double delta;
	static int majorTickCounter;
	struct timespec ts_start;

public:
	static unsigned int fps;
	static unsigned int lastFps;

	static void initTimer();
	static void updateTimer();

	static bool shallTick();
	static bool isMajorTick();

	static struct tm* getTimeOfDay();
	static double getAliveTime();
	static double getTickInterpolation();
	static double getDelta();
	static float getFDelta();
};

#endif /* TIMEMANAGER_H_ */
