/*
 * Shader.h
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#ifndef CLIENT_SHADERS_SHADER_H_
#define CLIENT_SHADERS_SHADER_H_

#include <string>
#include <glm/detail/type_mat.hpp>

class Shader
{
protected:
	std::string shaderName;
	unsigned int programId;

	int uniformTimeLocation;

public:
	static Shader* currentShader;

	Shader(const std::string& name);
	virtual ~Shader();

	void init(unsigned int id);

	virtual void use();
	virtual void stop();
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) = 0;
	virtual void onResize(int width, int height) {};
	const std::string& getShaderName() const;
	unsigned int getProgramId();

protected:
	virtual void bindAttributesAndUniforms() = 0;
	void bindAttribute(unsigned int index, const std::string& attributeName);
	void bindUniformLocation(const std::string& uniformName, int* locationOutput);
};

#endif /* CLIENT_SHADERS_SHADER_H_ */
