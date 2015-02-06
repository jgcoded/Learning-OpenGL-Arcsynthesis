#include "OpenGLHelper.h"

/* Location of shaders */
#define SRC_DIR "C:/Users/julio/projects/LearnOpenGL/Tut04/"

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

	/* Enable culling */

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	/* Create buffer */

	/*
	72 points in a cube

	+Z
	|
	|
   O|_______ +Y
	/
   /
   +X

*/
	const float vertexData[] = {

		0.25f, 0.25f, -1.25f, 1.0f,
		0.25f, -0.25f, -1.25f, 1.0f,
		-0.25f, 0.25f, -1.25f, 1.0f,

		0.25f, -0.25f, -1.25f, 1.0f,
		-0.25f, -0.25f, -1.25f, 1.0f,
		-0.25f, 0.25f, -1.25f, 1.0f,

		0.25f, 0.25f, -2.75f, 1.0f,
		-0.25f, 0.25f, -2.75f, 1.0f,
		0.25f, -0.25f, -2.75f, 1.0f,

		0.25f, -0.25f, -2.75f, 1.0f,
		-0.25f, 0.25f, -2.75f, 1.0f,
		-0.25f, -0.25f, -2.75f, 1.0f,

		-0.25f, 0.25f, -1.25f, 1.0f,
		-0.25f, -0.25f, -1.25f, 1.0f,
		-0.25f, -0.25f, -2.75f, 1.0f,

		-0.25f, 0.25f, -1.25f, 1.0f,
		-0.25f, -0.25f, -2.75f, 1.0f,
		-0.25f, 0.25f, -2.75f, 1.0f,

		0.25f, 0.25f, -1.25f, 1.0f,
		0.25f, -0.25f, -2.75f, 1.0f,
		0.25f, -0.25f, -1.25f, 1.0f,

		0.25f, 0.25f, -1.25f, 1.0f,
		0.25f, 0.25f, -2.75f, 1.0f,
		0.25f, -0.25f, -2.75f, 1.0f,

		0.25f, 0.25f, -2.75f, 1.0f,
		0.25f, 0.25f, -1.25f, 1.0f,
		-0.25f, 0.25f, -1.25f, 1.0f,

		0.25f, 0.25f, -2.75f, 1.0f,
		-0.25f, 0.25f, -1.25f, 1.0f,
		-0.25f, 0.25f, -2.75f, 1.0f,

		0.25f, -0.25f, -2.75f, 1.0f,
		-0.25f, -0.25f, -1.25f, 1.0f,
		0.25f, -0.25f, -1.25f, 1.0f,

		0.25f, -0.25f, -2.75f, 1.0f,
		-0.25f, -0.25f, -2.75f, 1.0f,
		-0.25f, -0.25f, -1.25f, 1.0f

	};

	GLuint positionsBufferObject = ogh_CreateBuffer(GL_ARRAY_BUFFER,
		vertexData);

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
	if (vsOffsetUniformLoc == -1)
		cout << "Could not find offset uniform " << endl;


	GLint perspectiveMatrixUnif = glGetUniformLocation(shaderProgram, "perspectiveMatrix");
	if (perspectiveMatrixUnif == -1)
		cout << "Could find perspectiveMatrix uniform" << endl;
	
	float fFrustrumScale = 1.0f;
	float fzNear = 0.0f;
	float fzFar = 1.0f;

	float theMatrix[16];
	::memset(theMatrix, 0, sizeof(float)* 16);
	theMatrix[0] = fFrustrumScale;
	theMatrix[5] = fFrustrumScale;
	theMatrix[10] = (fzFar + fzNear) / (fzNear - fzFar);
	theMatrix[14] = (2 * fzFar * fzNear) / (fzNear - fzFar);
	theMatrix[11] = -1.0f;

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, theMatrix);
	glUseProgram(0);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		// camera position
		glUniform2f(vsOffsetUniformLoc, 0.5f, 0.5f);

		glBindBuffer(GL_ARRAY_BUFFER, positionsBufferObject);

		glVertexAttribPointer(vertexShaderPositionsAttribLoc,
			sizeof(vertexData[0]), GL_FLOAT, false, 0, 0);

		glEnableVertexAttribArray(vertexShaderPositionsAttribLoc);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexData) / 12);

		glDisableVertexAttribArray(vertexShaderPositionsAttribLoc);

		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ogh_Deinit();

	return 0;
}
