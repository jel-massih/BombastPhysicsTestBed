#include "gl_headers.h"
#include "App.h"
#include "Cube.h"
#include <gl\glut.h>

class CollisionDemo : public App
{
	Cube m_cube;

public:
	CollisionDemo();

	virtual const char* GetTitle();

	virtual void Update();
	virtual void Render();

	virtual void SetCamera();
};

CollisionDemo::CollisionDemo()
{
	m_cube.particle.SetPosition(0.0f, 2.0f, 0.0f);
	m_cube.particle.SetMass(1.0f);
	m_cube.particle.SetDamping(0.001f);
}

const char* CollisionDemo::GetTitle()
{
	return "Bombast Physics Ballistics Demo";
}

void CollisionDemo::Update()
{
	App::Update();

	float duration = (float)m_pTimer->GetFrameTime() * 0.001f;
	if (duration <= 0.0f) return;

	m_cube.particle.Simulate(m_pTimer->GetTime() * 0.001f, duration);
}

void CollisionDemo::Render()
{
	//Clear viewport & set camera direction
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(-25.0, 8.0, 5.0,
		0.0, 5.0, 22.0,
		0.0, 1.0, 0.0);

	m_cube.Render();
}

void CollisionDemo::SetCamera()
{
	App::SetCamera();

	glClearColor(0.35f, 0.35f, 0.35f, 1);
}

App* GetApp()
{
	return new CollisionDemo();
}