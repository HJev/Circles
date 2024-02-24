#define GLEW_STATIC

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <../Circle.h>
#include <string>
#include <fstream>
#include <stdexcept>

unsigned int* compileShaders(const std::string&, const std::string&);
std::string createShader(const std::string& ShaderPath);
GLuint* createProgram(const std::string& vertex, const std::string& fragment);


int main() {
	GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "There was a problem initilizing glfw";
		return -1;
	}

	window = glfwCreateWindow(500, 500, "Test", NULL, NULL);
	
	if (!window) {
		glfwTerminate();
		std::cout << "There was a problem creating the window";
		return -1;
	}

	glfwMakeContextCurrent(window);
	//// window created 
	//// start glew
	if (glewInit() != GLEW_OK) {
		std::cout << "There was a problem with glew";
		return -1;
	}
	/////////////////

	Circle A = Circle::Circle(0.1f, Circle::vec2{ {0,0} });
	float test[9] = { 0 };
	for (int i = 0; i < 6; i++)
	{
		test[i] = A.getVertx().v[i];
	}
	float vertexs[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(A.getVertx().v), A.getVertx().v, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	
	unsigned int *program = createProgram("Shaders/Vertex.shader", "Shaders/Fragment.shader");
	
	

	while (!glfwWindowShouldClose(window)) {
		/* get render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle



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
unsigned int* compileShaders(const std::string &shader) {
	const char* Source = createShader(shader).c_str();
	unsigned int Sha = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Sha, 1, &Source, NULL);
	int isCompiled = 0;
	glGetShaderiv(Sha, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(Sha, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* errorLog = new char[maxLength];
		glGetShaderInfoLog(Sha, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(Sha); // Don't leak the shader.
		std::cout << errorLog << std::endl;
		delete errorLog;
		throw std::runtime_error(std::string("ShaderProblem on") + shader.c_str());
		return;
	}
	return &Sha;
}
GLuint* createProgram(const std::string& vertex, const std::string& fragment) {
	GLuint* vertexShader = compileShaders(vertex);
	GLuint* fragmentShader = compileShaders(fragment);
	GLuint program = glCreateProgram();
	glAttachShader(program, *vertexShader);
	glAttachShader(program, *fragmentShader);
	return &program;
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
