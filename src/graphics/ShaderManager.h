#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <stdint.h>
#include <unordered_map>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void Reload();

	uint32_t GetShader(const char* name);

private:
	void Load();
	void Unload();

	std::unordered_map<uint32_t, uint32_t> m_shaders;
};

#endif