#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL 3.5"
#define VERTEX_BUFFER_ID 1
#define INFO_LOG_SIZE 512
#define VAO_ONE 1

//Main functions
int initializeGLFW(GLFWwindow* &pWindow);

int setupBasicShaders(unsigned int& shaderProgram);

void setupVBOTriangle(unsigned int& vbo);

void setupTriangle1(unsigned int& vbo1);

void setupTriangle2(unsigned int& vbo2);

void setupEVORectangle(unsigned int& vbo, unsigned int& ebo);


void setupVAOOne(unsigned int& vao);


void processInput(GLFWwindow* window);

bool compileShader(const char* sourceCode, GLenum type, unsigned int& shaderOut);

bool linkShaders(unsigned int vertexShader, unsigned int fragmentShader, unsigned int& shaderProgramOut);


//Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);



int main() {

	
	GLFWwindow* pWindow = nullptr;
	if (!initializeGLFW(pWindow)) {
		return 1;
	}

	unsigned int shaderProgram;
	if (!setupBasicShaders(shaderProgram)) {
		return 1;
	}

	unsigned int vao = 0;
	setupVAOOne(vao);

	unsigned int vbo1 = 0;

	setupTriangle1(vbo1);

	unsigned int vao2 = 0;
	setupVAOOne(vao2);
	unsigned int vbo2 = 0;
	setupTriangle2(vbo2);

	//unsigned int ebo = 0;
	//setupEVORectangle(vbo,ebo);




	//Main loop
	while (!glfwWindowShouldClose(pWindow)) {

		//Process input
		processInput(pWindow);

		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		//Get events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(pWindow);
		

	}


	//Clean up
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &vbo2);
	glDeleteProgram(shaderProgram);
	

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


int setupBasicShaders(unsigned int & shaderProgram) {

	//Logging
	int success = false;
	char infoLog[INFO_LOG_SIZE];



	unsigned int vertexShader;
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	success = compileShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader);

	if (!success) {
		glGetShaderInfoLog(vertexShader, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "[ERROR] : Vertex shader compilation failed!\n  >> " << infoLog << std::endl;
		return 0;
	}
	std::cout << "Vertex shader compiled successfully!" << std::endl;



	unsigned int fragmentShader;
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	success = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "[ERROR] : Fragment shader compilation failed!\n >> " << infoLog << std::endl;
		return 0;
	}
	std::cout << "Fragment shader compiled successfully!" << std::endl;



	//Link shaders
	shaderProgram = 0;
	if (!linkShaders(vertexShader, fragmentShader, shaderProgram)) {
		return 0;
	}
	else {
		std::cout << "Shaders linked successfully!" << std::endl;
	}

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
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	
	};

	float vertices2[] = {
		// second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};


	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	//glGenBuffers(2, &vbo2);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo2);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// Setup vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
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

void setupVAOOne(unsigned int & vao) {
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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
