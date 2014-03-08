#include "OpenGLHelper.h"

/* Location of shaders */
#define SRC_DIR "C:/Users/julio/projects/LearnOpenGL/Tut03/"

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{

	GLFWwindow* window;

	if (ogh_Init() < 0)
		getchar();

	window = glfwCreateWindow(640, 480, "Learn OpenGL", NULL,
		ogh_GetSharedContext());

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resize);

	/* Create buffer */

	const float vertexData[] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f
	};

	GLuint positionsBufferObject = ogh_CreateBuffer(GL_ARRAY_BUFFER, vertexData);

	/* Create shader */

	vector<GLuint> shaders(2);
	
	shaders.push_back(
		ogh_CreateShader(SRC_DIR "VertexShader.txt", GL_VERTEX_SHADER));
	shaders.push_back(
		ogh_CreateShader(SRC_DIR "FragmentShader.txt", GL_FRAGMENT_SHADER));
	
	/* Create program */

	GLuint shaderProgram = ogh_CreateProgram(shaders);

	GLint vertexShaderPositionsAttribLoc = glGetAttribLocation(shaderProgram,
		"position");

	GLint vsOffsetUniformLoc = glGetUniformLocation(shaderProgram, "offset");
	if(vsOffsetUniformLoc == -1){
		cout << "Could not find offset uniform " << endl;
		
	}


	float change = 0.0f;

	while(!glfwWindowShouldClose(window))
	{
		/* Update */

		change+= 0.0001f;
		if (change > 0.5f)
			change = 0;

		/* End update */

		/* Render */

		glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glUniform2f(vsOffsetUniformLoc, change, change);

		glBindBuffer(GL_ARRAY_BUFFER, positionsBufferObject);

		glVertexAttribPointer(vertexShaderPositionsAttribLoc,
			4, GL_FLOAT, false, 0, 0);

		glEnableVertexAttribArray(vertexShaderPositionsAttribLoc);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(vertexShaderPositionsAttribLoc);

		glUseProgram(0);

		/* End render */

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ogh_Deinit();

	return 0;
}
