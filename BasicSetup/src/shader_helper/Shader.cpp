#include "Shader.h"


Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) : vertexShaderPath_(vertexShaderPath), fragmentShaderPath_(fragmentShaderPath)
{
	shaderProgram_ = 0;
}

Shader::~Shader()
{
	if (shaderProgram_ != 0)
		glDeleteProgram(shaderProgram_);

	if (vertexShaderSource_ != nullptr)
		delete[] vertexShaderSource_;

	if (fragmentShaderSource_ != nullptr)
		delete[] fragmentShaderSource_;

}

bool Shader::initialize()
{
	if (!readShaderSource())
		return false;

	if (!linkShaders())
		return false;

	return true;
}

void Shader::use()
{
	glUseProgram(shaderProgram_);
}

unsigned int Shader::getProgram()
{
	return shaderProgram_;
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgram_, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram_, name.c_str()), value);
}



bool Shader::readShaderSource()
{
	if (vertexShaderPath_.empty() || fragmentShaderPath_.empty()) {
		std::cout << "Error: Shader paths are empty" << std::endl;
		return false;
	}

	std::ifstream vertexShaderFile(vertexShaderPath_);
	std::ifstream fragmentShaderFile(fragmentShaderPath_);

	//Set exceptions
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::string vertexShaderSource, fragmentShaderSource;
	
	std::stringstream vertexShaderStream, fragmentShaderStream;
	try {

		//Open files
		if (!vertexShaderFile.is_open() || !fragmentShaderFile.is_open()) {
			std::cout << "Error opening shader files" << std::endl;
			return false;
		}

		//Read files
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		//Close files
		vertexShaderFile.close();
		fragmentShaderFile.close();

		//Convert streams to strings
		vertexShaderSource = vertexShaderStream.str();
		fragmentShaderSource = fragmentShaderStream.str();

	}
	catch (std::exception e) {
		std::cout << "Error reading shader files" << std::endl;
		return false;
	}

	vertexShaderSource_ = new char[vertexShaderSource.size() + 1];
	strcpy_s(const_cast<char*>(vertexShaderSource_), vertexShaderSource.size() + 1, vertexShaderSource.c_str());

	fragmentShaderSource_ = new char[fragmentShaderSource.size() + 1];
	strcpy_s(const_cast<char*>(fragmentShaderSource_), fragmentShaderSource.size() + 1, fragmentShaderSource.c_str());

	return true;
}

bool Shader::compileShader(const char* sourceCode, GLenum type, unsigned int& shaderOut) {
	//Create and compile vertex shader
	shaderOut = glCreateShader(type);
	unsigned int sourceCodeNum = 1;

	glShaderSource(shaderOut, sourceCodeNum, &sourceCode, NULL);
	glCompileShader(shaderOut);

	//Check for compilation errors
	int success = 0;
	char infoLog[INFO_LOG_SIZE];

	glGetShaderiv(shaderOut, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderOut, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "[ERROR] : Shader compilation failed!\n  >> " << infoLog << std::endl;
	}

	return success;
}

bool Shader::linkShaders()
{
	//Logging
	int success = false;
	char infoLog[INFO_LOG_SIZE];

	unsigned int vertexShader;
	success = compileShader(vertexShaderSource_, GL_VERTEX_SHADER, vertexShader);

	if (!success) {
		glGetShaderInfoLog(vertexShader, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "[ERROR] : Vertex shader compilation failed!\n  >> " << infoLog << std::endl;
		return false;
	}

	unsigned int fragmentShader;
	success = compileShader(fragmentShaderSource_, GL_FRAGMENT_SHADER, fragmentShader);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "[ERROR] : Fragment shader compilation failed!\n >> " << infoLog << std::endl;
		return false;
	}

	if (vertexShaderSource_ != nullptr) {
		delete[] vertexShaderSource_;
		vertexShaderSource_ = nullptr;
	}
		

	if (fragmentShaderSource_ != nullptr) {
		delete[] fragmentShaderSource_;
		fragmentShaderSource_ = nullptr;
	}
		

	//Link shaders
	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader);
	glAttachShader(shaderProgram_, fragmentShader);
	glLinkProgram(shaderProgram_);

	//Check for linking errors
	success = 0;

	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram_, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "[ERROR] : Shader program linking failed!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;

}