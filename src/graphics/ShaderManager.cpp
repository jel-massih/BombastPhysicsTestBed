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
	DWORD error = 0;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(shaderDirectory, &ffd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("ERROR: Invalid Shader directory\n");
		return;
	}

	do
	{
		const char* filename = ffd.cFileName;
		const int len = strlen(filename);

		if (len > 5 &&
			filename[len - 5] == '.' &&
			filename[len - 4] == 'v' &&
			filename[len - 3] == 'e' &&
			filename[len - 2] == 'r' &&
			filename[len - 1] == 't')
		{
			const int MaxPath = 2048;
			char filenameWithoutExtension[MaxPath];
			strcpy_s(filenameWithoutExtension, ffd.cFileName);
			filenameWithoutExtension[len - 5] = '\0';

			char vertexShaderPath[MaxPath];
			char fragmentShaderPath[MaxPath];

			sprintf_s(vertexShaderPath, "%s/%s.vert", shaderDirectory, filenameWithoutExtension);
			sprintf_s(fragmentShaderPath, "%s/%s.frag", shaderDirectory, filenameWithoutExtension);
			
			unsigned int shader = loadShader(vertexShaderPath, fragmentShaderPath);
			if (shader == 0)
			{
				continue;
			}

			uint32_t key = hashString(filenameWithoutExtension);
			m_shaders.insert(std::pair<uint32_t, uint32_t>(key, shader));
		}

	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
}

void ShaderManager::Unload()
{
	for (auto it = m_shaders.begin(); it != m_shaders.end(); it++)
	{
		deleteShader(it->second);
	}

	m_shaders.clear();
}