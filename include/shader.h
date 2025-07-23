#pragma once

#include "base_renderer.h"
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Shader {
	Shader(const char* vertexShaderFilename, const char* fragmentShaderFileName);
	virtual ~Shader();

	void bind();
	void unbind();

	GLint getUniformLocation(const std::string& name);

	void setUniform1i(const std::string& name, int v0);

	void setUniform1f(const std::string& name, float v0);
	void setUniform2f(const std::string& name, float v0, float v1);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void setUniform2iv(const std::string& name, GLsizei count, glm::vec2 vector);

	void setUniform2fv(const std::string& name, GLsizei count, glm::vec2 vector);
	void setUniform3fv(const std::string& name, GLsizei count, glm::vec3 vector);
	void setUniform4fv(const std::string& name, GLsizei count, glm::vec4 vector);

	void setUniformMatrix4fv(const std::string& name, GLsizei count, GLboolean transpose, glm::mat4 matrix);
	GLuint getShaderId();

private:
	GLuint compile(std::string shaderSource, GLenum type, const char* shaderFileName);
	std::string parse(const char* filename);
	GLuint createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);

	std::unordered_map<std::string, GLint> uniformLocations = { {} };
	GLuint shaderId = 0;
};