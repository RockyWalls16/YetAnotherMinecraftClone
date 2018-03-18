#ifndef CLIENT_RENDER_LIGHT_LIGHTCACHE_H_
#define CLIENT_RENDER_LIGHT_LIGHTCACHE_H_

#define MAX_DIR_LIGHT 4

#include <vector>
#include <client/render/light/DirectionalLight.h>

class Light;

class LightCache
{
private:
	static std::vector<DirectionalLight*> directionalLights;

public:
	static void addDirectionalLight(DirectionalLight* light);

	static std::vector<DirectionalLight*>& getDirectionalLights();

	static void cleanLights();
};

#endif