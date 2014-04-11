#include "OpenGLHelper.h"

/* Location of shaders */
#define SRC_DIR "C:/Users/julio/projects/LearnOpenGL/Tut05/"

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

const int numberOfVertices = 36;

#define RIGHT_EXTENT 0.8f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT 0.20f
#define MIDDLE_EXTENT 0.0f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -1.25f
#define REAR_EXTENT -1.75f

#define GREEN_COLOR 0.75f, 0.75f, 1.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.5f, 0.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertexData[] = {
	//Object 1 positions
	LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
	LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
	RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
	RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,

	LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,
	LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
	RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
	RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

	LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
	LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
	LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

	RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,
	RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
	RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

	LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,
	LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
	RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,
	RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

	//	0, 2, 1,
	//	3, 2, 0,

	//Object 2 positions
	TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
	MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
	MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
	TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,

	BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
	MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
	MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
	BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

	TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
	MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
	BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,

	TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,
	MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
	BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

	BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
	TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
	TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,
	BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

	//Object 1 colors
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,

	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,

	RED_COLOR,
	RED_COLOR,
	RED_COLOR,

	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,

	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,

	//Object 2 colors
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,

	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,

	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,

	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,

	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,
};

const GLshort indexData[] =
{
	0, 2, 1,
	3, 2, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	11, 13, 12,

	14, 16, 15,
	17, 16, 14,
};

int main(int argc, char* argv[])
{

	GLFWwindow* window;

	if (ogh_Init() < 0)
		exit(1);

	window = glfwCreateWindow(640, 480, "Tut05", NULL,
		ogh_GetSharedContext());

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resize);

	/* create buffers */

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vao;

	vertexBuffer = ogh_CreateBuffer(GL_ARRAY_BUFFER, vertexData);

	indexBuffer = ogh_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, indexData);

	/* create the vao */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	size_t colorDataOffset = sizeof(float)* 3 * numberOfVertices;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // vertex data

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)colorDataOffset); // color data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindVertexArray(0);


	/* shaders */

	vector<GLuint> shaders(2);

	shaders.push_back(
		ogh_CreateShader(SRC_DIR "VertexShader.txt", GL_VERTEX_SHADER));
	shaders.push_back(
		ogh_CreateShader(SRC_DIR "FragmentShader.txt", GL_FRAGMENT_SHADER));

	GLuint shaderProgram = ogh_CreateProgram(shaders);

	GLint vertexShaderPositionsAttribLoc = glGetAttribLocation(shaderProgram,
		"position");

	GLint vsOffsetUniformLoc = glGetUniformLocation(shaderProgram, "offset");
	if (vsOffsetUniformLoc == -1)
		cout << "Could not find offset uniform " << endl;

	GLint perspectiveMatrixUnif = glGetUniformLocation(shaderProgram, "perspectiveMatrix");
	if (perspectiveMatrixUnif == -1)
		cout << "Could not find perspectiveMatrix uniform" << endl;

	float fFrustrumScale = 1.0f;
	float fzNear = 1.0f;
	float fzFar = 3.0f;

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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glEnable(GL_DEPTH_CLAMP);

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		glUniform3f(vsOffsetUniformLoc, 0.0f, 0.0f, 0.0f);

		glDrawElements(GL_TRIANGLES, sizeof(indexData), GL_UNSIGNED_SHORT, 0);

		glUniform3f(vsOffsetUniformLoc, 0.0f, 0.0f, 0.0f);

		glDrawElementsBaseVertex(GL_TRIANGLES, sizeof(indexData), GL_UNSIGNED_SHORT,
			0, numberOfVertices / 2);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ogh_Deinit();


	return 0;
}