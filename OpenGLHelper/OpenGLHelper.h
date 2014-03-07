#ifndef OPENGL_HELPER_H
#define OPENGL_HELPER_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdarg>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/* Initialize GLFW and glew */
int ogh_Init();

/* Deinitialize GLFW and glew */
int ogh_Deinit();

/* Get the shared context */
GLFWwindow* ogh_GetSharedContext();

/* Load a shader from a text document */
GLuint ogh_CreateShader(const char* path, GLenum shaderType);

/* Create a glsl program from shaders */
GLuint ogh_CreateProgram(vector<GLuint> shaders);

/* Log a shader error to a file */
void ogh_LogShaderError(GLenum shaderType, const char* path, GLchar** shaderData, GLuint shaderHandle);

/* Log a shader program error to a file */
void ogh_LogShaderProgramError(GLuint shaderProgram);

/* Create a buffer object */
template<typename T>
GLuint ogh_CreateBuffer(GLenum target, const T* data, int size)
{
	GLuint bufferObject;

	glGenBuffers(1, &bufferObject);

	glBindBuffer(target, bufferObject);

	glBufferData(target, size * sizeof(T), data, GL_STATIC_DRAW);

	glBindBuffer(target, 0);

	return bufferObject;
}

#endif /* OPENGL_HELPER_H */
