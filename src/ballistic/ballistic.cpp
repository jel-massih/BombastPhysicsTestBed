#include "../gl_headers.h"
#include "../App.h"
#include "../Cubes.h"

class BallisticDemo : public App
{
public:
	BallisticDemo();

	virtual const char* GetTitle();

	virtual void Update();
	virtual void Render();
};

BallisticDemo::BallisticDemo()
{

}

const char* BallisticDemo::GetTitle()
{
	return "Bombast Physics Ballistics Demo";
}

void BallisticDemo::Update()
{
	App::Update();
}

void BallisticDemo::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(-25.0, 8.0, 5.0,
		0.0, 5.0, 22.0,
		0.0, 1.0, 0.0);

	glColor3f(0.75f, 0.75f, 0.75f);
	glBegin(GL_LINES);
	for (int i = 0; i < 20; i++)
	{
		glVertex3f(-5.0f, 0.0f, i * 10.f);
		glVertex3f(5.0f, 0.0f, i * 10.f);
	}
	glEnd();
}

App* GetApp()
{
	return new BallisticDemo();
}