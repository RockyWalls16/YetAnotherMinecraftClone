#ifndef CLIENT_RENDER_LIGHT_LIGHTCACHE_H_
#define CLIENT_RENDER_LIGHT_LIGHTCACHE_H_

#define MAX_LIGHT 50

class Light;

class LightCache
{
private:
	Light * lights[MAX_LIGHT];
};

#endif