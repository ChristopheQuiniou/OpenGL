#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include "shader_helper\Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL 3.5"
#define VERTEX_BUFFER_ID 1

#define VAO_ONE 1

//Main functions
int initializeGLFW(GLFWwindow* &pWindow);

void setupEVORectangle(unsigned int& vbo, unsigned int& ebo, unsigned int& vao);

void processInput(GLFWwindow* window);

bool imageLoading(unsigned int& texture,const char* path);


//Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


static float xPosition = 0.0f;

int main() {
	
	GLFWwindow* pWindow = nullptr;
	if (!initializeGLFW(pWindow)) {
		return 1;
	}


	Shader* shader = new Shader("E:\\Dev\\OpenGL\\BasicSetup\\shaders\\vertex_shader_with_color_and_texture.glsl", "E:\\Dev\\OpenGL\\BasicSetup\\shaders\\fragment_shader_vertex_color_and_texture.glsl");
	if (!shader->initialize()) {
		std::cerr << "[ERROR] : Shader initialization failed!" << std::endl;
		return 1;
	}

	unsigned int vbo1 = 0;
	unsigned int ebo1 = 0;
	unsigned int vao1 = 0;
	setupEVORectangle(vbo1, ebo1, vao1);

	unsigned int texture0 = 0;
	const char* texture0Path = "E:\\Dev\\OpenGL\\BasicSetup\\res\\HardDance.jpg";
	if (!imageLoading(texture0, texture0Path))
		return 1;

	unsigned int texture1 = 0;
	const char* texture1Path = "E:\\Dev\\OpenGL\\BasicSetup\\res\\Darling.jpg";
	if (!imageLoading(texture1, texture1Path))
		return 1;
	

	//Main loop
	while (!glfwWindowShouldClose(pWindow)) {

		//Process input
		processInput(pWindow);

		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		shader->use();
		shader->setFloat("horizontalOffset", xPosition);

		shader->setInt("aTexture1", 0);
		shader->setInt("aTexture2", 1);
		
		glBindVertexArray(vao1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	

		//Get events and swap buffers
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}


	//Clean up
	glDeleteVertexArrays(1, &vao1);
	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &ebo1);

	
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

bool imageLoading(unsigned int& texture, const char* path) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "Image loaded successfully!" << std::endl;
	} else {
		std::cerr << "Failed to load image!" << std::endl;
		return false;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	
	return true;
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

void setupEVORectangle(unsigned int& vbo, unsigned int& ebo,unsigned int& vao)
{
	float vertices[] = {
		0.5f, 0.5f,0.0f, 1.0f, 0.0f,0.0f, 1.0f,1.0f,
		0.5f, -0.5f,0.0f, 0.0f, 1.0f,0.0f, 1.0f,0.0f,
		-0.5f, -0.5f,0.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f,
		-0.5f, 0.5f,0.0f, 1.0f, 1.0f,0.0f, 0.0f,1.0f,
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Setup vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  // Position attribute
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // Color attribute
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // Texture coordinate attribute
	glEnableVertexAttribArray(2);

	
}
