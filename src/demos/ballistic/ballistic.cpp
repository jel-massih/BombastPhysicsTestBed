#include "gl_headers.h"
#include "App.h"
#include "graphics/Cubes.h"
#include <gl\glut.h>

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
	//Clear Viewport & set camera dir
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(-25.0, 8.0, 5.0,
		0.0, 5.0, 22.0,
		0.0, 1.0, 0.0);

	//Sphere at firing point and Shadow
	glColor3f(0.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0.0f, 1.5f, 0.0f);
	glutSolidSphere(0.1f, 5, 5);
	glTranslatef(0.0f, -1.5f, 0.0f);
	glColor3f(0.75f, 0.75f, 0.75f);
	glScalef(1.0f, 0.1f, 1.0f);
	glutSolidSphere(0.1f, 5, 5);
	glPopMatrix();

	//Draw Scale Lines
	glColor3f(0.75f, 0.75f, 0.75f);
	glBegin(GL_LINES);
	for (int i = 0; i < 20; i++)
	{
		glVertex3f(-5.0f, 0.0f, i * 10.f);
		glVertex3f(5.0f, 0.0f, i * 10.f);
	}
	glEnd();

	//Render the bullets

	//Render Description
	glColor3f(0.0f, 0.0f, 0.0f);
	RenderText(10.0f, 34.0f, "Click: Fire\n1-4: Change Ammo");
}

App* GetApp()
{
	return new BallisticDemo();
}