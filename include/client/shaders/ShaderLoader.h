/*
 * ShaderLoader.h
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#ifndef CLIENT_SHADERS_SHADERLOADER_H_
#define CLIENT_SHADERS_SHADERLOADER_H_

#include <client/shaders/Shader.h>
#include <client/shaders/ShaderCache.h>
#include <string>
#include <type_traits>

enum ShaderType : char
{
	VERTEX,
	FRAGMENT
};

class ShaderLoader
{
public:
	template<class T>
	static T* registerShader(const std::string& name);

	static void initShader(Shader& shader);
private:
	static unsigned int loadShaderFile(ShaderType type, std::string path);
};

// Template bug fix
template<class T>
T* ShaderLoader::registerShader(const std::string& name)
{
	static_assert(std::is_base_of<Shader, T>::value, "T must derive from Shader");

	T* shader = new T(name);
	ShaderCache::addShader(*shader);
	return shader;
}

#endif /* CLIENT_SHADERS_SHADERLOADER_H_ */
