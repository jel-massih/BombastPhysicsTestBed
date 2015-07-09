#include "App.h"

#include "gl_headers.h"

void App::Initialize()
{
	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	SetCamera();
	glDisable(GL_DEPTH_TEST);
}

void App::Shutdown()
{
}

void App::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	glVertex2i(1, 1);
	glVertex2i(639, 319);
	glEnd();
}

void App::Update()
{
}

void App::WindowResized(int width, int height)
{
	if (height <= 0)
	{
		height = 1;
	}

	m_width = width;
	m_height = height;
	
	SetCamera();
}

const char* App::GetTitle()
{
	return "Bombast Physics Demo";
}

void App::SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)m_width / (float)m_height, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void App::KeyEvent(int key, int scanCode, int action, int mods)
{
}

void App::CharEvent(unsigned int code)
{
}

void App::MouseClick(int button, int action, int mods)
{
}

void App::MouseDrag(int x, int y)
{
}