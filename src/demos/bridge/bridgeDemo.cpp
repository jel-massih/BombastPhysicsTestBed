#include "gl_headers.h"
#include "App.h"
#include <gl\glut.h>

class BridgeDemo : public App
{
public:
	BridgeDemo();

	virtual const char* GetTitle();

	virtual void Update();
	virtual void Render();

	virtual void SetCamera();
};

BridgeDemo::BridgeDemo()
{
}

const char* BridgeDemo::GetTitle()
{
	return "Bombast Physics Bridge Demo";
}

void BridgeDemo::Update()
{
	App::Update();

	float duration = (float)m_pTimer->GetFrameTime() * 0.001f;
	if (duration <= 0.0f) return;

}

void BridgeDemo::Render()
{
	//Clear viewport & set camera direction
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 1.85f, 8, 0, 0.5f, 0, 0, 1, 0);

}

void BridgeDemo::SetCamera()
{
	App::SetCamera();

	glClearColor(0.35f, 0.35f, 0.35f, 1);
}

App* GetApp()
{
	return new BridgeDemo();
}