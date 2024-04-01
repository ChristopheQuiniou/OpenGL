#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include "shader_helper\Shader.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL 3.5"
#define VERTEX_BUFFER_ID 1

#define VAO_ONE 1

//Main functions
int initializeGLFW(GLFWwindow* &pWindow);

void setupVBOTriangle(unsigned int& vbo);

void setupTriangle1(unsigned int& vbo1);

void setupTriangle2(unsigned int& vbo2);

void setupEVORectangle(unsigned int& vbo, unsigned int& ebo);


void setupVAO1(unsigned int& vao1);
void setupVAO2(unsigned int& vao2);


void processInput(GLFWwindow* window);

bool compileShader(const char* sourceCode, GLenum type, unsigned int& shaderOut);

bool linkShaders(unsigned int vertexShader, unsigned int fragmentShader, unsigned int& shaderProgramOut);


//Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


static float xPosition = 0.0f;

int main() {
	
	GLFWwindow* pWindow = nullptr;
	if (!initializeGLFW(pWindow)) {
		return 1;
	}


	Shader* shader = new Shader("E:\\Dev\\OpenGL\\BasicSetup\\shaders\\vertex_shader_with_color.glsl", "E:\\Dev\\OpenGL\\BasicSetup\\shaders\\fragment_shader_vertex_color.glsl");
	if (!shader->initialize()) {
		std::cerr << "[ERROR] : Shader initialization failed!" << std::endl;
		return 1;
	}


	unsigned int vao1 = 0;
	setupVAO1(vao1);

	unsigned int vbo1 = 0;
	setupTriangle1(vbo1);

	//Main loop
	while (!glfwWindowShouldClose(pWindow)) {

		//Process input
		processInput(pWindow);

		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
		shader->setFloat("horizontalOffset", xPosition);
		
		//Nice green color
		/*float timeValue = glfwGetTime();
		float greenValue = -1 * (cos(timeValue) / 2.1f) + 0.6f;
		int vertexColorLocation = glGetUniformLocation(shader->getProgram(), "color");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/


		glBindVertexArray(vao1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
		//Get events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(pWindow);
		
	}


	//Clean up
	glDeleteBuffers(1, &vao1);
	glDeleteBuffers(1, &vbo1);

	
	delete shader;

	glfwTerminate();

	return 0;
}


int initializeGLFW(GLFWwindow* &pWindow) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//For mac
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (pWindow == nullptr) {
		std::cerr << "[ERROR] : The creation of the window failed!" << std::endl;
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(pWindow);


	//Initialization of GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "[ERROR] : Failed to initialize GLAD!" << std::endl;
		return 0;
	}

	glViewport(0, 0, WINDOW_HEIGHT, WINDOW_HEIGHT);


	//Set a callback to widow resizing
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
	return 1;
}



//Handle window resizing and update viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


//Handle window input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		xPosition -= 0.001f;
	} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		xPosition += 0.001f;
	}
}

void setupVBOTriangle(unsigned int & vbo) {

	float vertices[] = {
		0.5f, 0.5f,0.0f,
		-0.5f, 0.5f,0.0f,
		0.0f, -0.5f,0.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Setup vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}


void setupTriangle1(unsigned int& vbo1) {

	
	float vertices1[] = {
		-0.9f, -0.5f, 0.0f,1.0f,0.0f,0.0f,  // left 
		-0.0f, -0.5f, 0.0f,0.0f,1.0f,0.0f,  // right
		-0.45f, 0.5f, 0.0f,0.0f,0.0f,1.0f,  // top 
	
	};


	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	//glGenBuffers(2, &vbo2);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo2);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// Setup vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
}


void setupTriangle2(unsigned int& vbo2) {

	float vertices2[] = {
		// second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// Setup vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void setupEVORectangle(unsigned int& vbo, unsigned int& ebo)
{
	float vertices[] = {
		0.5f, 0.5f,0.0f,
		-0.5f, 0.5f,0.0f,
		-0.5f, -0.5f,0.0f,
		0.5f, -0.5f,0.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Setup vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void setupVAO1(unsigned int & vao1) {
	
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);

}

void setupVAO2(unsigned int& vao2) {

	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);

}

bool compileShader(const char* sourceCode,GLenum type,unsigned int& shaderOut ) {


	//Create and compile vertex shader
	shaderOut = glCreateShader(type);
	unsigned int sourceCodeNum = 1;
	
	glShaderSource(shaderOut, sourceCodeNum, &sourceCode,NULL);
	glCompileShader(shaderOut);

	//Check for compilation errors
	int success = 0;
	char infoLog[INFO_LOG_SIZE];

	glGetShaderiv(shaderOut, GL_COMPILE_STATUS, &success);

	return success;
}


bool linkShaders(unsigned int vertexShader, unsigned int fragmentShader, unsigned int& shaderProgramOut) {

	shaderProgramOut = glCreateProgram();
	glAttachShader(shaderProgramOut, vertexShader);
	glAttachShader(shaderProgramOut, fragmentShader);
	glLinkProgram(shaderProgramOut);

	//Check for linking errors
	int success = 0;
	char infoLog[INFO_LOG_SIZE];

	glGetProgramiv(shaderProgramOut, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgramOut, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "[ERROR] : Shader program linking failed!\n" << infoLog << std::endl;
	}

	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return success;
}
