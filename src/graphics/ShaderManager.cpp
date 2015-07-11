#include "ShaderManager.h"
#include "Core.h"

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
	return key;
}

void ShaderManager::Load()
{

}

void ShaderManager::Unload()
{

}