#ifndef CUBES_H
#define CUBES_H

#include "vectorial\vectorial.h"
#include <stdint.h>

const int MaxCubes = 1024;

struct CubeInstance
{
	float r, g, b, a;
	vectorial::mat4f model;
	vectorial::mat4f modelView;
	vectorial::mat4f modelViewProjection;
};

const int MaxCubeShadowVerts = 1024 * 32;

class CubesRender
{
public:
	CubesRender();
	~CubesRender();

	void RenderCubes();
	void RenderCubeShadows();
	void RenderShadowQuad();
	
private:
	void Initialize();

	uint32_t m_cubesVao;
	uint32_t m_cubesVbo;
	uint32_t m_cubesIbo;
	uint32_t m_cubesInstanceBuffer;

	uint32_t m_shadowVao;
	uint32_t m_shadowVbo;

	uint32_t m_maskVao;
	uint32_t m_maskVbo;

	vectorial::vec3f m_shadowVertices[MaxCubeShadowVerts];

	CubeInstance m_instanceData[MaxCubes];
};

struct CubesInternal
{
	CubesRender render;

	void Initialize();
	void Free();

	void AddCube(const vectorial::vec3f& position);
	void Update();

	bool Clear();
	void Render();
};
#endif