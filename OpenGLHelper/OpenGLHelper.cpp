#include "OpenGLHelper.h"

static GLFWwindow* _ogh_sharedContext;

int ogh_Init()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

	_ogh_sharedContext = glfwCreateWindow(16, 16, "", NULL, NULL);
	if (_ogh_sharedContext == NULL) {
		cout << "Shared context is null" << endl;
		return -1;
	}

	glfwMakeContextCurrent(_ogh_sharedContext);

	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

	GLenum error = glewInit();

	if (error != GLEW_OK) {
		glfwTerminate();
		cout << "Glew init failed" << endl;
		return -1;
	}

	return 1;
}

int ogh_Deinit()
{

	glfwTerminate();

	if (_ogh_sharedContext != NULL)
		glfwDestroyWindow(_ogh_sharedContext);

	return 1;
}

GLFWwindow* ogh_GetSharedContext()
{
	return _ogh_sharedContext;
}

GLuint ogh_CreateShader(const char* path, GLenum shaderType)
{
	GLchar* shaderData;
	int fileLength;
	ifstream fileStream(path);
	GLuint shaderHandle;

	if (!fileStream.is_open())
		return -1;

	fileStream.seekg(0, fileStream.end);
	fileLength = (int)fileStream.tellg();
	fileStream.seekg(0, fileStream.beg);

	shaderData = new char[fileLength + 1];

	if (shaderData == 0)
		return -1; // no memory

	unsigned int i = 0;
	while (fileStream.good())
	{
		shaderData[i] = fileStream.get();
		if (!fileStream.eof())
			++i;
	}

	shaderData[fileLength] = 0;

	shaderHandle = glCreateShader(shaderType);
	const GLchar* sdata = shaderData;
	glShaderSource(shaderHandle, 1, &sdata, NULL);
	glCompileShader(shaderHandle);

	GLint status;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
		ogh_LogShaderError(shaderType, path, &shaderData, shaderHandle);

	fileStream.close();

	delete shaderData;

	return shaderHandle;
}


GLuint ogh_CreateProgram(vector<GLuint> shaders)
{
	GLint status;
	GLuint shaderProgram = glCreateProgram();

	for (unsigned int i = 0; i < shaders.size(); ++i)
		glAttachShader(shaderProgram, shaders[i]);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
		ogh_LogShaderProgramError(shaderProgram);

	for (unsigned int i = 0; i < shaders.size(); ++i) {
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	return shaderProgram;
}


void ogh_LogShaderError(GLenum shaderType, const char* path, GLchar** shaderData, GLuint shaderHandle)
{
	ofstream error("Error.txt", ofstream::app);

	GLint logLength;
	glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
	GLchar* log = new GLchar[logLength + 1];
	glGetShaderInfoLog(shaderHandle, logLength, NULL, log);

	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		error << "Vertex shader compile error" << endl;
		break;

	case GL_FRAGMENT_SHADER:
		error << "Fragment shader compile error" << endl;
		break;

	default:
		error << "Unidentified shader type: " << shaderType << endl;
	}

	error << path << endl;
	error << *shaderData << endl;
	error << log << endl;

	error << "=======================================================" << endl;

	error.close();

	delete[] log;
}


void ogh_LogShaderProgramError(GLuint shaderProgram)
{
	ofstream error("Error.txt", ofstream::app);

	GLint logLength;
	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
	char* log = new char[logLength + 1];
	glGetProgramInfoLog(shaderProgram, logLength, NULL, log);

	error << "could not link program" << endl;
	error << log << endl;

	error.close();

	delete[] log;
}
