#include "infrastructure/opengl/shader.h"
#include <fstream>
#include <iostream>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

Shader::Shader(const char* vertexShaderFilename, const char* fragmentShaderFileName) {
	shaderId = createShader(vertexShaderFilename, fragmentShaderFileName);
}

Shader::~Shader() {
	glDeleteProgram(shaderId);
}

void Shader::bind() {
	glUseProgram(shaderId);
}

void Shader::unbind() {
	glUseProgram(0);
}

GLuint Shader::getShaderId() const {
	return shaderId;
}

std::string Shader::parse(const char* filename) const {
	FILE* file;
	if (fopen_s(&file, filename, "rb") != 0) {
		std::cout << "File " << filename << " not found" << std::endl;
		return "";
	}

	std::string contents;
	fseek(file, 0, SEEK_END);
	size_t filesize = ftell(file);
	rewind(file);
	contents.resize(filesize);

	fread(&contents[0], 1, filesize, file);
	fclose(file);

	return contents;
}

GLuint Shader::compile(const std::string shaderSource, 
	const GLenum type, const const char* shaderFilename) const 
{
	GLuint shaderId = glCreateShader(type);
	const char* src = shaderSource.c_str();
	glShaderSource(shaderId, 1, &src, 0);
	glCompileShader(shaderId);

	// check for compilation errors
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		int length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length]; // create compilation errror message on heap
		glGetShaderInfoLog(shaderId, length, &length, message);
		std::string shaderType;
		std::cout << shaderFilename << " shader compilation error: \n" << message << std::endl;
		delete[] message; //delete message from heap
		glDeleteShader(shaderId);
		return 0;
	}

	// return result if no errors
	return shaderId;
}


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
