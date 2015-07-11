#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void Reload();

	unsigned int GetShader(const char* name);

private:
	void Load();
	void Unload();


};

#endif