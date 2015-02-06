#include "OpenGLHelper.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <chrono>

using namespace glm;

typedef chrono::time_point<chrono::system_clock> Time;
typedef chrono::duration<chrono::system_clock::rep, chrono::system_clock::period> Duration;
typedef chrono::time_point<chrono::system_clock>::clock Clock;

/* Location of shaders */
#define SRC_DIR "C:/Users/julio/projects/LearnOpenGL/Tut06/"

const int numberOfVertices = 24;

#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f

#define YELLOW_COLOR 1.0f, 1.0f, 0.0f, 1.0f
#define CYAN_COLOR 0.0f, 1.0f, 1.0f, 1.0f
#define MAGENTA_COLOR 	1.0f, 0.0f, 1.0f, 1.0f

const float vertexData[] =
{
	//Front
	+1.0f, +1.0f, +1.0f,
	+1.0f, -1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,

	//Top
	+1.0f, +1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,
	-1.0f, +1.0f, -1.0f,
	+1.0f, +1.0f, -1.0f,

	//Left
	+1.0f, +1.0f, +1.0f,
	+1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, +1.0f,

	//Back
	+1.0f, +1.0f, -1.0f,
	-1.0f, +1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,

	//Bottom
	+1.0f, -1.0f, +1.0f,
	+1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, +1.0f,

	//Right
	-1.0f, +1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, +1.0f, -1.0f,


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
	RED_COLOR,

	YELLOW_COLOR,
	YELLOW_COLOR,
	YELLOW_COLOR,
	YELLOW_COLOR,

	CYAN_COLOR,
	CYAN_COLOR,
	CYAN_COLOR,
	CYAN_COLOR,

	MAGENTA_COLOR,
	MAGENTA_COLOR,
	MAGENTA_COLOR,
	MAGENTA_COLOR,
};

const GLshort indexData[] =
{
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,
	18, 19, 16,

	20, 21, 22,
	22, 23, 20,
};

GLuint shaderProgram;
GLuint cameraToClipUniformMatrix;
mat4 cameraToClipMatrix(0.0f);

float CalcFrustumScale(float fFovDeg)
{
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	float fFovRad = fFovDeg * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
}

void resize(GLFWwindow* window, int width, int height)
{
	cout << "I'm called" << endl;
	float frustumScale = CalcFrustumScale(45.0f);
	cameraToClipMatrix[0].x = frustumScale * (height / (float)width);
	cameraToClipMatrix[0].y = frustumScale;

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(cameraToClipUniformMatrix, 1, GL_FALSE, value_ptr(cameraToClipMatrix));
	glUseProgram(0);

	glViewport(0, 0, width, height);
}

void translate(mat4* transform, float x, float y, float z)
{
	vec3 translation(x, y, z);
	(*transform)[3] = vec4(translation, 1.0f);
}

void scale(mat4* transform, float x, float y, float z)
{
	(*transform)[0].x = x;
	(*transform)[1].y = y;
	(*transform)[2].z = z;
	(*transform)[3].w = 1.0f;
}

void rotate(mat4* transform, float x, float y, float z)
{
	// degrees to radians
	//radians per degree
	//mat4 transform(1.0f);

	mat4 rotX(1.0f);
	mat4 rotY(1.0f);
	mat4 rotZ(1.0f);

	static float radPerDegree = 2 * pi<float>() / 360.0f;
	float radX = x * radPerDegree;
	float radY = y * radPerDegree;
	float radZ = z * radPerDegree;

	rotX[1].y = cosf(radX);
	rotX[1].z = sinf(radX);
	rotX[2].y = -sinf(radX);
	rotX[2].z = cosf(radX);

	rotY[0].x = cosf(radY);
	rotY[0].z = -sinf(radY);
	rotY[2].x = sinf(radY);
	rotY[2].z = cosf(radY);

	rotZ[0].x = cosf(radZ);
	rotZ[0].y = sinf(radZ);
	rotZ[1].x = -sinf(radZ);
	rotZ[1].y = cosf(radZ);

	*transform = rotX * rotY * rotZ;
}

int main(int argc, char* argv[])
{
	GLFWwindow* window;

	if (ogh_Init() < 0)
		exit(1);

	window = glfwCreateWindow(640, 480, "Tut06", NULL,
		ogh_GetSharedContext());

	glfwMakeContextCurrent(window);


	/* Make shaders */

	shaderProgram;
	vector<GLuint> shaders(2);
	shaders.push_back(ogh_CreateShader(SRC_DIR "VertexShader.txt", GL_VERTEX_SHADER));
	shaders.push_back(ogh_CreateShader(SRC_DIR "FragmentShader.txt", GL_FRAGMENT_SHADER));
	
	shaderProgram = ogh_CreateProgram(shaders);

	GLuint positionVSAttribute = glGetAttribLocation(shaderProgram, "position");
	GLuint colorVSAttribute = glGetAttribLocation(shaderProgram, "color");
	cameraToClipUniformMatrix = glGetUniformLocation(shaderProgram, "cameraToClipMatrix");
	GLuint modelToCameraUniformMatrix = glGetUniformLocation(shaderProgram, "modelToCameraMatrix");
	glfwSetWindowSizeCallback(window, resize);
	printf("%d %d %d %d %d\n", positionVSAttribute, colorVSAttribute,
		cameraToClipUniformMatrix, modelToCameraUniformMatrix, shaderProgram);

	/* Set camera to clip space matrix */

	float fFrustumScale = CalcFrustumScale(45.0f);
	float fzNear = 1.0f; float fzFar = 45.0f;

	cameraToClipMatrix[0].x = fFrustumScale;
	cameraToClipMatrix[1].y = fFrustumScale;
	cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
	cameraToClipMatrix[2].w = -1.0f;
	cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(cameraToClipUniformMatrix, 1, GL_FALSE, value_ptr(cameraToClipMatrix));
	glUseProgram(0);

	/* Make buffers */

	GLuint vb, ib, vao;
	vb = ogh_CreateBuffer(GL_ARRAY_BUFFER, vertexData);
	ib = ogh_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, indexData);

	/* Make Vertex Array Object */

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;
	/* bind the data buffer that holds both attribute pos and color data */
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glEnableVertexAttribArray(0); /* enable position in VS */
	glEnableVertexAttribArray(1); /* enable color in VS */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	/* finally bind the index buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBindVertexArray(0);

	printf("%d %d %d\n", vb, ib, vao);

	/* Culling and depth bit testing*/
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	mat4 transformMatrix(1.0f);
	mat4 scaleM(1.0f);
	mat4 transM(1.0f);
	mat4 rotM(1.0f);
	Time startTime = Clock::now();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		//update uniforms and draw

		float dTime = ((Duration)(Clock::now() - startTime)).count();

		float transX = 2 * cosf(dTime / 1.5e6);
		float transY = 2 * sinf(dTime / 1.5e6);
		float transZ = -20 + 10 * cosf(dTime / 1.5e6);
		
		float scaleX = abs(transX) + 1;
		float scaleY = abs(transY) + 1;
		float scaleZ = abs(transZ) / 10.0f;

		float rotX =  cosf(dTime / 7.5e6) * 360.0f;
		float rotY = sinf(dTime / 2e6) * 360.0f;
		float rotZ = 0;// tanf(dTime / 10e6) * 360.0f;

		translate(&transM, transX, transY, transZ);
		scale(&scaleM, scaleX, scaleY, scaleZ);
		rotate(&rotM, rotX, rotY, rotZ);

		transformMatrix = transM * rotM * scaleM;

		glUniformMatrix4fv(modelToCameraUniformMatrix, 1, GL_FALSE, value_ptr(transformMatrix));
		glDrawElements(GL_TRIANGLES, sizeof(indexData), GL_UNSIGNED_SHORT, 0);

		translate(&transM, -9.0f,3.0f, -30.0f);
		transformMatrix = transM;
		glUniformMatrix4fv(modelToCameraUniformMatrix, 1, GL_FALSE, value_ptr(transformMatrix));
		glDrawElements(GL_TRIANGLES, sizeof(indexData), GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
		glUseProgram(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ogh_Deinit();

	return 0;
}
