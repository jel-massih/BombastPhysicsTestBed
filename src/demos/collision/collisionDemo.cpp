#include "gl_headers.h"
#include "App.h"
#include <gl\glut.h>
#include "View.h"

class CollisionDemo : public App
{
	View view;
	Client client;

public:
	CollisionDemo();

	virtual const char* GetTitle();

	virtual void Update();
	virtual void Render();

	virtual void SetCamera();
};

CollisionDemo::CollisionDemo()
{
	view.Initialize(client);
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

	client.Update(duration);
	view.Update(duration);
}

void CollisionDemo::Render()
{
	//Clear viewport & set camera direction
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 1.85f, 8, 0, 0.5f, 0, 0, 1, 0);

	view.Render();
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