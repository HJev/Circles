#define GLEW_STATIC

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "../Circle.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <chrono>

unsigned int compileShaders(const std::string&, GLenum);
std::string createShader(const std::string& ShaderPath);
GLuint createProgram(const std::string& vertex, const std::string& fragment);

void ChangeSize(GLFWwindow*, int, int);
void processInput(GLFWwindow*, int, int, int, int);



int main() {
	GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "There was a problem initilizing glfw";
		return -1;
	}

	window = glfwCreateWindow(800, 800, "Test", NULL, NULL);
	
	
	if (!window) {
		glfwTerminate();
		std::cout << "There was a problem creating the window";
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, ChangeSize);
	glfwSetKeyCallback(window, processInput);

	glfwMakeContextCurrent(window);
	//// window created 
	//// start glew
	if (glewInit() != GLEW_OK) {
		std::cout << "There was a problem with glew";
		return -1;
	}
	/////////////////

	


	Circle small = Circle::Circle({0.1f , 0.0f}, Circle::vec2{{0,0}});
	small.setVelocity({ 0.001f,0 });
	Circle big = Circle::Circle({0.9f, 0.87f}, {0.0f,0});
	GLuint smallArrayObject, smallVertexBuffer;
	GLuint bigArrayObject, bigVertexBuffer;

	glGenBuffers(1, &smallVertexBuffer);
	glGenVertexArrays(1, &smallArrayObject);
	glGenBuffers(1, &bigVertexBuffer);
	glGenVertexArrays(1, &bigArrayObject);

	glBindVertexArray(smallVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, smallVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(small.getVertx().v), small.getVertx().v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	glEnableVertexAttribArray(0);
	

	glBindVertexArray(bigArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, bigVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(big.getVertx().v), big.getVertx().v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	
	
	unsigned int program = createProgram("Shaders/Vertex.shader", "Shaders/Circle.shader");

	int i = 0;
	while (!glfwWindowShouldClose(window)) {
		/* get render here */
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		if (!(small.checkCircleColision(big, 0.01f) || small.checkWallColision(0.01f))) {
			small.move(0.01f);
			big.setRadius({ big.getRadius()[0] - 0.0005f, big.getRadius()[1] - 0.0005f });
		}
		else {
			big.setRadius({ big.getRadius()[0] + 0.1f, big.getRadius()[1] + 0.1f});
		};
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);


		glBindBuffer(GL_ARRAY_BUFFER, smallVertexBuffer);
		small.setVelocity({ small.getVelocity().v[0], small.getVelocity().v[1] - 0.001f });
		int radiusLoc = glGetUniformLocation(program, "radius");
		glProgramUniform2fv(program, radiusLoc, 1, small.getRadius().v);
		int centerLoc = glGetUniformLocation(program, "center");
		glProgramUniform2fv(program, centerLoc, 1, small.getCenter().v);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(small.getVertx().v), small.getVertx().v);
		
		glBindVertexArray(smallArrayObject);
		int ColorLoc = glGetUniformLocation(program, "color");
		glProgramUniform3f(program, ColorLoc, 1, 1, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glBindVertexArray(0);


		glProgramUniform2fv(program, radiusLoc, 1, big.getRadius().v);
		glProgramUniform2fv(program, centerLoc, 1, big.getCenter().v);
		glProgramUniform3f(program, ColorLoc, 1, 0, 1);

		glBindBuffer(GL_ARRAY_BUFFER, bigVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big.getVertx().v), big.getVertx().v);

		glBindVertexArray(bigArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		


		// wait for a frame to pass
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		while (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < 750) {
			end = std::chrono::system_clock::now();
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and Process events */
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}

/**
* Compiles both the vertex and fragment shaders
* @param vertex
* @param fragment
*/
unsigned int compileShaders(const std::string &shader, GLenum type) {
	std::string Source = createShader(shader);
	const char* ID = Source.c_str();
	unsigned int Sha = glCreateShader(type);
	glShaderSource(Sha, 1, &ID, NULL);
	glCompileShader(Sha);

	int isCompiled = 0;
	glGetShaderiv(Sha, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(Sha, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* errorLog = new char[maxLength];
		glGetShaderInfoLog(Sha, maxLength, NULL, errorLog);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(Sha); // Don't leak the shader.
		std::cout << "There was an error in:" << ((type == GL_VERTEX_SHADER) ? "Vertex Shader\n" : "Fragment Shader\n") << errorLog << std::endl;
	}
	return Sha;
}
GLuint createProgram(const std::string& vertex, const std::string& fragment) {
	GLuint vertexShader = compileShaders(vertex, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShaders(fragment, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	
	int isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* errorLog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, NULL, errorLog);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteProgram(program); // Don't leak the shader.
		std::cout << errorLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}
std::string createShader(const std::string& ShaderPath) {
	std::ifstream File;
	File.open(ShaderPath, std::ifstream::in);
	if (!File.is_open()) {
		std::cout << "The file is not open" << std::endl;
	}
	std::string result = "";
	char temp;
	while (File.get(temp))
	{
		result += temp;
	}
	File.close();
	return result;
}


void ChangeSize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}

}
