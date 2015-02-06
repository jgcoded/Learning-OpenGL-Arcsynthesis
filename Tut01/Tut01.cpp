#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

/* Vertex Shader */

const string MyVertexShader =
"#version 120\n"
"attribute vec4 position;\n"
"void main()\n"
"{\n"
"    gl_Position = position;\n"
"}\n";


const string MyFragmentShader =
"#version 120\n"
"void main()\n"
"{\n"
"    float lerpValue = gl_FragCoord.y / 480;"
"    gl_FragColor =  mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), "
"vec4(0.0f, 0.0f, 0.0f, 1.0f), lerpValue);\n"
"}\n";


void resize(GLFWwindow* window, int width, int height)
{
	cout << "I'm called" << endl;
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
	/* Set up GLFW */
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cout << "GLFW create window failed" << endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resize);

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		glfwTerminate();
		//        delete window;
		cout << "Glew init failed " << endl;
		return -1;
	}

	/* End set up GLFW */

	const char *version = (const char *)glGetString(GL_VERSION);
	printf("%s", version);

	float change = 0.0f;
	const float vertexPositions[] = {
		-0.5f + change, -0.5f, 0.0f, 1.0f,
		0.0f + change, 0.5f, 0.0f, 1.0f,
		0.5f + change, -0.5f, 0.0f, 1.0f
	};

	GLuint positionsBufferObject;

	glGenBuffers(1, &positionsBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionsBufferObject);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create shaders */

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShaderObject == 0)
	{
		glfwTerminate();
		cout << "vertex shader error" << endl;
		cout << glGetError() << endl;
		//delete window;
		return -1;
	}

	const char* vertexShaderData = MyVertexShader.c_str();
	glShaderSource(vertexShaderObject, 1, &vertexShaderData, NULL);

	glCompileShader(vertexShaderObject);

	GLint status;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		cout << "Vertex shader compilation fail" << endl;
		GLint logLength;
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* log = new GLchar[logLength + 1];
		glGetShaderInfoLog(vertexShaderObject, logLength, NULL, log);
		cout << log << endl;

		glfwTerminate();
		delete[] log;
		//delete window;
		return -1;
	}

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderData = MyFragmentShader.c_str();

	glShaderSource(fragmentShaderObject, 1, &fragmentShaderData, NULL);

	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		cout << "Fragment shader compilation fail" << endl;
		GLint logLength;
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* log = new GLchar[logLength + 1];
		glGetShaderInfoLog(fragmentShaderObject, logLength, NULL, log);
		cout << log << endl;

		glfwTerminate();
		delete[] log;
		//delete window;
		return -1;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderObject);
	glAttachShader(shaderProgram, fragmentShaderObject);

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		cout << "could not link program" << endl;
		GLint logLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength + 1];
		glGetProgramInfoLog(shaderProgram, logLength, NULL, log);

		glfwTerminate();
		delete[] log;
		//delete window;
	}

	glDetachShader(shaderProgram, vertexShaderObject);
	glDetachShader(shaderProgram, fragmentShaderObject);

	glDeleteShader(vertexShaderObject);
	glDeleteShader(fragmentShaderObject);


	GLint positionAttribLocation = glGetAttribLocation(shaderProgram, "position");

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */

		glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindBuffer(GL_ARRAY_BUFFER, positionsBufferObject);

		glVertexAttribPointer(positionAttribLocation, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(positionAttribLocation);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(positionAttribLocation);
		glUseProgram(0);

		/* End render */

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &positionsBufferObject);
	glfwTerminate();
	//    delete window;

	return 0;
}
