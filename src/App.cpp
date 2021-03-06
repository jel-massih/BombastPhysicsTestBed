#include "App.h"

#include <cstring>
#include "gl_headers.h"
#include <gl\glut.h>
#include <cassert>
#include <cstdarg>
#include <stdio.h>

void App::Initialize()
{
	m_pTimer = new Timer();
	if (m_pTimer) {
		m_pTimer->Initialize();
	}

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	SetCamera();
	glDisable(GL_DEPTH_TEST);
}

void App::Shutdown()
{
	if (m_pTimer) { delete m_pTimer; };
	m_pTimer = 0;
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
	m_pTimer->Frame();
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

void App::RenderText(float x, float y, const char* text)
{
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (double)m_width, 0.0, (double)m_height, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	size_t textLength = strlen(text);

	glRasterPos2f(x, y);
	for (const char* letter = text; letter < text + textLength; letter++)
	{
		if (*letter == '\n') {
			y -= 12.0f;
			glRasterPos2f(x, y);
		}
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *letter);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
}

MassAggregateApp::MassAggregateApp(unsigned int particleCount)
	: m_world(particleCount * 10)
{
	m_particles = BP_NEW bPhysics::BpParticle[particleCount];
	for (unsigned i = 0; i < particleCount; i++)
	{
		m_world.GetParticles().push_back(m_particles + i);
	}
	
	m_groundContactGenerator.Init(&m_world.GetParticles());
	m_world.GetContactGenerators().push_back(&m_groundContactGenerator);
}

MassAggregateApp::~MassAggregateApp()
{
	delete[] m_particles;
}

void MassAggregateApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 3.5, 8.0, 
			  0.0, 3.5, 0.0,
			  0.0, 1.0, 0.0);

	glColor3f(0, 0, 0);

	bPhysics::BpParticleWorld::Particles& particles = m_world.GetParticles();
	for (auto it = particles.begin(); it != particles.end(); it++)
	{
		const bPhysics::BpVec3& pos = (*it)->GetPosition();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(0.1f, 20, 10);
		glPopMatrix();
	}
}

void MassAggregateApp::Update()
{
	App::Update();
	m_world.StartFrame();

	float dt = (float)m_pTimer->GetFrameTime() * 0.001f;
	if (dt <= 0.0f)
		return;

	m_world.RunPhysics(dt);
}

void MassAggregateApp::Initialize()
{
	App::Initialize();
}
