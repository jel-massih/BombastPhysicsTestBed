#include "Cubes.h"
#include "gl_headers.h"

CubesRender::CubesRender()
{
	m_cubesVao = 0;
	m_cubesVbo = 0;
	m_cubesIbo = 0;
	m_cubesInstanceBuffer = 0;
	m_shadowVao = 0;
	m_shadowVbo = 0;
	m_maskVao = 0;
	m_maskVbo = 0;
}

CubesRender::~CubesRender()
{
	glDeleteVertexArrays(1, &m_cubesVao);
	glDeleteBuffers(1, &m_cubesVbo);
	glDeleteBuffers(1, &m_cubesIbo);
	glDeleteBuffers(1, &m_cubesInstanceBuffer);

	m_cubesVao = 0;
	m_cubesVbo = 0;
	m_cubesIbo = 0;
	m_cubesInstanceBuffer = 0;

	glDeleteVertexArrays(1, &m_shadowVao);
	glDeleteBuffers(1, &m_shadowVbo);

	m_shadowVao = 0;
	m_shadowVbo = 0;

	glDeleteVertexArrays(1, &m_maskVao);
	glDeleteBuffers(1, &m_maskVbo);

	m_maskVao = 0;
	m_maskVbo = 0;
}

void CubesRender::Initialize()
{

}