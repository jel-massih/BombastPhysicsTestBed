#include "ShaderManager.h"
#include "Core.h"

#include <Windows.h>

ShaderManager::ShaderManager()
{
	Load();
}

ShaderManager::~ShaderManager()
{
	Unload();
}

void ShaderManager::Reload()
{
	printf("Reloading Shaders\n");
	Unload();
	Load();
}

uint32_t ShaderManager::GetShader(const char* name)
{
	const uint32_t key = hashString(name);
	uint32_t shader = m_shaders.at(key);

	if (shader)
	{
		return shader;
	}

	const uint32_t defaultKey = hashString("default");
	return m_shaders.at(defaultKey);
}

void ShaderManager::Load()
{
	const char* shaderDirectory = "data/shaders";

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(shaderDirectory, &ffd);
}

void ShaderManager::Unload()
{
}