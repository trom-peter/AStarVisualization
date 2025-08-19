#include "infrastructure/opengl/shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

Shader::Shader(const char* vertexShaderFilename, const char* fragmentShaderFileName) {
	shaderId = createShader(vertexShaderFilename, fragmentShaderFileName);
}

Shader::~Shader() {
	glDeleteProgram(shaderId);
}

void Shader::bind() const {
	glUseProgram(shaderId);
}

void Shader::unbind() const {
	glUseProgram(0);
}

GLuint Shader::getShaderId() const {
	return shaderId;
}

// Parse the given shader file
std::string Shader::parse(const std::string& filename) const {
	std::ifstream shaderFile(filename, std::ios::binary);

	if (!shaderFile.is_open()) {
		std::cerr << "Failed to open shader file: " + filename << std::endl;
	}

	std::ostringstream buffer;
	buffer << shaderFile.rdbuf();
	return buffer.str();
}

// Compile the given shader file
GLuint Shader::compile(const std::string& shaderSource, GLenum type, const std::string& shaderFilename) const 
{
	GLuint shaderId = glCreateShader(type);

	if (shaderId == 0) {
		std::cerr << "Failed to create shader for: " + shaderFilename << std::endl;
	}

	// Compile shader
	const char* source = shaderSource.c_str();
	glShaderSource(shaderId, 1, &source, 0);
	glCompileShader(shaderId);

	// Check shader compilation status
	int compileResult;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);
	if (compileResult != GL_TRUE) {
		GLint length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		glGetShaderInfoLog(shaderId, length, &length, message);
		glDeleteShader(shaderId);

		std::string shaderType;
		std::cerr << "Shader compilation error for : " << shaderFilename << "\n" << message << std::endl;

		return 0;
	}

	// Return result if no errors
	return shaderId;
}

// Create an OpenGL shader program based on vertex shader and fragment shader file names
GLuint Shader::createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename) const {
	std::string vertexShaderSource = parse(vertexShaderFilename);
	std::string fragmentShaderSource = parse(fragmentShaderFilename);

	GLuint program = glCreateProgram();
	GLuint vs = compile(vertexShaderSource, GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fs = compile(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShaderFilename);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

GLint Shader::getUniformLocation(const std::string& name) {
	auto location = uniformLocations.find(name);

	if (location != uniformLocations.end()) {
		return location->second;
	}
	else {
		GLint location = glGetUniformLocation(shaderId, name.c_str());
		uniformLocations[name] = location;
		return location;
	}
}

void Shader::setUniform1i(const std::string& name, const int v0) {
	glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform1f(const std::string& name, const float v0) {
	glUniform1f(getUniformLocation(name), v0);
}

void Shader::setUniform2f(const std::string& name, const float v0, const float v1) {
	glUniform2f(getUniformLocation(name), v0, v1);
}

void Shader::setUniform3f(const std::string& name, const float v0, const float v1, const float v2) {
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string& name, const float v0, 
	const float v1, const float v2, const float v3) 
{
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform2iv(const std::string& name, const GLsizei count, const glm::vec2 vector) {
	glUniform2iv(getUniformLocation(name), count, (int*)&vector.x);
}

void Shader::setUniform2fv(const std::string& name, const GLsizei count, const glm::vec2 vector) {
	glUniform2fv(getUniformLocation(name), count, (float*)&vector.x);
}

void Shader::setUniform3fv(const std::string& name, const GLsizei count, const glm::vec3 vector) {
	glUniform3fv(getUniformLocation(name), count, (float*)&vector.x);
}

void Shader::setUniform4fv(const std::string& name, const GLsizei count, const glm::vec4 vector) {
	glUniform4fv(getUniformLocation(name), count, (float*)&vector.x);
}

void Shader::setUniformMatrix4fv(const std::string& name, const GLsizei count, 
	const GLboolean transpose, const glm::mat4 matrix) 
{
	glUniformMatrix4fv(getUniformLocation(name), count, transpose, &matrix[0][0]);
}
