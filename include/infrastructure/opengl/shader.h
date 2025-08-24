#pragma once
#include <string>
#include <unordered_map>
#include "infrastructure/opengl/base_renderer.h"
#include "glm/glm.hpp"

struct Shader {
	Shader(const char* vertexShaderFilename, const char* fragmentShaderFileName);
	virtual ~Shader();

	void bind() const;
	void unbind() const;

	GLint getUniformLocation(const std::string& name);

	void setUniform1i(const std::string& name, int v0);

	void setUniform1f(const std::string& name, const float v0);
	void setUniform2f(const std::string& name, const float v0, const float v1);
	void setUniform3f(const std::string& name, const float v0, const float v1, const float v2);
	void setUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3);

	void setUniform2iv(const std::string& name, const GLsizei count, const glm::vec2 vector);

	void setUniform2fv(const std::string& name, const GLsizei count, const glm::vec2 vector);
	void setUniform3fv(const std::string& name, const GLsizei count, const glm::vec3 vector);
	void setUniform4fv(const std::string& name, const GLsizei count, const glm::vec4 vector);

	void setUniformMatrix4fv(const std::string& name, const GLsizei count, 
		const GLboolean transpose, const glm::mat4 matrix);

	GLuint getShaderId() const;

private:
	GLuint compile(const std::string& shaderSource, GLenum type, const std::string& shaderFilename) const;
	std::string parse(const std::string&) const;
	GLuint createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename) const;

	std::unordered_map<std::string, GLint> uniformLocations = { {} };
	GLuint shaderId = 0;
};
