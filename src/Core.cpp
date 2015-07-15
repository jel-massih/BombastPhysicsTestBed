#include "Core.h"
#include "gl_headers.h"
#include <assert.h>
#include <cstdio>

uint32_t hashString(const char string[], uint32_t hash)
{
	assert(string);
	while (string != '\0')
	{
		char c = *string++;
		if ((c >= 'a') && (c <= 'z'))
		{
			c = c - 'a' + 'A';
		}
		hash += c;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	return hash;
}

bool loadTextFile(const char* filename, char* buffer, int bufferSize)
{
	FILE* file;
	fopen_s(&file, filename, "r");
	if (!file)
	{
		return false;
	}

	fseek(file, 0L, SEEK_END);
	uint64_t fileSize = ftell(file);
	fseek(file, 0L, SEEK_SET);

	if (fileSize+1 >= bufferSize)
	{
		fclose(file);
		return false;
	}

	fread(buffer, 1, fileSize, file);

	fclose(file);
	buffer[fileSize] = '\0';

	return true;
}

uint16_t loadShader(const char* filename, int type)
{
	GLuint shaderId = glCreateShader(type);

	static const int bufferSize = 256 * 1024;
	char buffer[bufferSize];
	if (!loadTextFile(filename, buffer, bufferSize))
	{
		printf("Error: Failed to load Shader");
		return 0;
	}

	const char* source = buffer;
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);

	GLint result = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		const int infoLogLength = 2048;
		char infoLog[infoLogLength];
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, infoLog);
		printf("====================================================\n"
			"Failed to Compile Shader: %s\n"
			"====================================================\n"
			"%s\n"
			"====================================================\n", filename, infoLog);
		glDeleteShader(shaderId);
		return 0;
	}

	return shaderId;
}

uint32_t loadShader(const char* vertexFilePath, const char* fragmentFilePath)
{
	uint32_t vertexShaderId = loadShader(vertexFilePath, GL_VERTEX_SHADER);
	if (!vertexShaderId)
	{
		return 0;
	}

	uint32_t fragmentShaderId = loadShader(fragmentFilePath, GL_FRAGMENT_SHADER);
	if (!fragmentShaderId)
	{
		glDeleteShader(vertexShaderId);
		return 0;
	}

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	GLint result = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		const int infoLogLength = 2048;
		char infoLog[infoLogLength];
		glGetShaderInfoLog(programId, infoLogLength, NULL, infoLog);
		printf("====================================================\n"
			"Failed to Link Shader: %s - %s\n"
			"====================================================\n"
			"%s\n"
			"====================================================\n", vertexFilePath, fragmentFilePath, infoLog);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
		glDeleteShader(programId);
		return 0;
	}
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	
	return programId;
}

void deleteShader(uint32_t shader)
{
	glDeleteProgram(shader);
}