/*
 * TimeManager.cpp
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#include <util/TimeManager.h>
#include <util/GLHeader.h>
#include <util/Logger.h>
#include <string>

unsigned int TimeManager::fps = 0;
unsigned int TimeManager::lastFps = 0;
double TimeManager::lastTime = 0;
double TimeManager::secondProgress = 0;
double TimeManager::tickInterpolation = 0;
double TimeManager::delta = 0;
double TimeManager::tickTime = 0;

void TimeManager::initTimer()
{
	TimeManager::lastTime = TimeManager::getAliveTime();
}

void TimeManager::updateTimer()
{
	fps++;

	double time = getAliveTime();
	delta = time - lastTime;
	secondProgress += delta;
	tickTime += delta;

	tickInterpolation = tickTime / TICK_DELAY;

	// FPS calculation
	if(secondProgress >= 1.0)
	{
		//Info("FPS: " + std::to_string(fps));

		lastFps = fps;
		secondProgress = 0.0;
		fps = 0;
	}

	lastTime = time;
}

bool TimeManager::shallTick()
{
	if(tickTime >= TICK_DELAY)
	{
		tickTime -= TICK_DELAY;
		tickInterpolation = tickTime / TICK_DELAY;

		return true;
	}

	return false;
}

struct tm* TimeManager::getTimeOfDay()
{
	time_t rawTime;
	time(&rawTime);

	return localtime(&rawTime);
}

double TimeManager::getAliveTime()
{
	return glfwGetTime();
}

double TimeManager::getTickInterpolation()
{
	return tickInterpolation;
}

double TimeManager::getDelta()
{
	return delta;
}

float TimeManager::getFDelta()
{
	return (float) delta;
}
