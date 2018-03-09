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
