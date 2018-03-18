#include <client/render/light/LightCache.h>

std::vector<DirectionalLight*> LightCache::directionalLights;

void LightCache::addDirectionalLight(DirectionalLight * light)
{
	directionalLights.push_back(light);
}

std::vector<DirectionalLight*>& LightCache::getDirectionalLights()
{
	return directionalLights;
}

void LightCache::cleanLights()
{
	for (DirectionalLight* light : directionalLights)
	{
		light->dirty = false;
	}
}
