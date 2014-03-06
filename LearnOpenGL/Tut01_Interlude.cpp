#include "OpenGLHelper.h"

/* Location of shaders */
#define SRC_DIR "C:/Users/julio/projects/LearnOpenGL/LearnOpenGL/"

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{

	GLFWwindow* window;

	if(!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Learn Open GL", NULL, NULL);

	if(!window)
	{
		glfwTerminate();
		cout << "GLFW window creation failed" << endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resize);

	GLenum error = glewInit();

	if(error != GLEW_OK)
	{
		glfwTerminate();
		cout << "Glew init failed" << endl;
		return -1;
	}

	/* Vertex data */

	const float vertexData[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
         0.0f, 0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f
	};

	GLuint positionsBufferObject = ogh_CreateBuffer(GL_ARRAY_BUFFER,
		vertexData, 12);

	/* Shaders */

	vector<GLuint> shaders(2);
	shaders.push_back(
		ogh_CreateShader(SRC_DIR "VertexShader.txt", GL_VERTEX_SHADER));
	shaders.push_back(
		ogh_CreateShader(SRC_DIR "FragmentShader.txt", GL_FRAGMENT_SHADER));
	

	/* Program */

	GLuint shaderProgram = ogh_CreateProgram(shaders);

	GLint vertexShaderPositionsAttribLocation = glGetAttribLocation(
		shaderProgram, "position");

	/* Main loop */

	while(!glfwWindowShouldClose(window))
	{

		/* Render here */

		glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindBuffer(GL_ARRAY_BUFFER, positionsBufferObject);

		glVertexAttribPointer(vertexShaderPositionsAttribLocation,
			4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(vertexShaderPositionsAttribLocation);

		glDrawArrays(GL_TRIANGLES, 0 , 3);

		glDisableVertexAttribArray(vertexShaderPositionsAttribLocation);

		glUseProgram(0);

		/* End Render */

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &positionsBufferObject);
	glfwTerminate();
	
}