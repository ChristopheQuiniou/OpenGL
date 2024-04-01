#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#define INFO_LOG_SIZE 512

class Shader
{
public:
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
	~Shader();

	bool initialize();
	void use();
	unsigned int getProgram();

	void setFloat(const std::string& name, float value) const;

private:
	bool readShaderSource();
	bool compileShader(const char* sourceCode, GLenum type, unsigned int& shaderOut);
	bool linkShaders();
	
	const char* vertexShaderSource_;
	const char* fragmentShaderSource_;
	std::string vertexShaderPath_;
	std::string fragmentShaderPath_;

	unsigned int shaderProgram_;

};

