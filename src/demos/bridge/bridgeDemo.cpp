#include "gl_headers.h"
#include "App.h"
#include <gl\glut.h>

#define ROD_COUNT 6
#define CABLE_COUNT 10
#define SUPPORT_COUNT 12

#define VERTEX_COUNT 12

#define BASE_MASS 1
#define EXTRA_MASS 10

class BridgeDemo : public MassAggregateApp
{
	bPhysics::BpParticleCableConstraint* supports;
	bPhysics::BpParticleCable* cables;
	bPhysics::BpParticleRod* rods;

	bPhysics::BpVec3 massPos;
	bPhysics::BpVec3 massDisplayPos;

	void UpdateAdditionalMass();

public:
	BridgeDemo();
	virtual ~BridgeDemo();

	virtual const char* GetTitle();

	virtual void Update();
	virtual void Render();

	virtual void KeyEvent(int key, int scanCode, int action, int mods);
};

BridgeDemo::BridgeDemo()
	: MassAggregateApp(VERTEX_COUNT), cables(nullptr), supports(nullptr), rods(nullptr)
{
	for (unsigned i = 0; i < VERTEX_COUNT; i++)
	{
		unsigned x = (i % VERTEX_COUNT) / 2;
		m_particles[i].SetPosition(bPhysics::f32(i / 2)*2.0f - 5.0f, 4, bPhysics::f32(i % 2)*2.0f - 1.0f);
		m_particles[i].SetVelocity(0, 0, 0);
		m_particles[i].SetDamping(0.9f);
		m_particles[i].SetAcceleration(bPhysics::BpVec3(0, -9.81, 0));
	}
	
	cables = new bPhysics::BpParticleCable[CABLE_COUNT];
	for (unsigned i = 0; i < CABLE_COUNT; i++)
	{
		cables[i].particle[0] = &m_particles[i];
		cables[i].particle[1] = &m_particles[i+2];
		cables[i].maxLength = 1.9f;
		cables[i].restitution = 0.3f;
		m_world.GetContactGenerators().push_back(&cables[i]);
	}

	supports = new bPhysics::BpParticleCableConstraint[SUPPORT_COUNT];
	for (unsigned i = 0; i < SUPPORT_COUNT; i++)
	{
		supports[i].particle = m_particles + i;
		supports[i].anchor = bPhysics::BpVec3(bPhysics::f32(i / 2) * 2.2f - 5.5f, 6, bPhysics::f32(i % 2)*1.6f - 0.8f);
		if (i < SUPPORT_COUNT / 2)
		{
			supports[i].maxLength = bPhysics::f32(i / 2)*0.5f + 3.0f;
		}
		else
		{
			supports[i].maxLength = 5.5f - bPhysics::f32(i / 2) * 0.5f;
		}

		supports[i].restitution = 0.5f;
		m_world.GetContactGenerators().push_back(&supports[i]);
	}

	rods = new bPhysics::BpParticleRod[ROD_COUNT];
	for (unsigned i = 0; i < ROD_COUNT; i++)
	{
		rods[i].particle[0] = &m_particles[i * 2];
		rods[i].particle[1] = &m_particles[i * 2 + 1];;
		rods[i].length = 2;
		m_world.GetContactGenerators().push_back(&rods[i]);
	}
	
	UpdateAdditionalMass();
}

BridgeDemo::~BridgeDemo()
{
	if (cables)
	{
		delete[] cables;
	}

	if (rods)
	{
		delete[] rods;
	}

	if (supports)
	{
		delete[] supports;
	}
}

void BridgeDemo::UpdateAdditionalMass()
{
	for (unsigned i = 0; i < VERTEX_COUNT; i++)
	{
		m_particles[i].SetMass(BASE_MASS);
	}

	int x = int(massPos.x);

	bPhysics::f32 xp = fmod(massPos.x, bPhysics::f32(1.0f));
	if (x < 0)
	{
		x = 0;
		xp = 0;
	}
	if (x >= 5)
	{
		x = 5;
		xp = 0;
	}

	int z = int(massPos.z);
	bPhysics::f32 zp = fmod(massPos.z, bPhysics::f32(1.0f));
	if (z < 0)
	{
		z = 0;
		zp = 0;
	}
	if (z >= 1)
	{
		z = 1;
		zp = 0;
	}

	massDisplayPos.Zero();

	m_particles[x * 2 + z].SetMass(BASE_MASS + EXTRA_MASS*(1 - xp)*(1 - zp));
	massDisplayPos += m_particles[x * 2 + z].GetPosition() * ((1 - xp)*(1 - zp));

	if (xp > 0)
	{
		m_particles[x * 2 + z + 2].SetMass(BASE_MASS + EXTRA_MASS*xp*(1 - zp));
		massDisplayPos += m_particles[x * 2 + z + 2].GetPosition() * (xp*(1 - zp));

		if (zp > 0)
		{
			m_particles[x * 2 + z + 3].SetMass(BASE_MASS + EXTRA_MASS*xp*zp);
			massDisplayPos += m_particles[x * 2 + z + 3].GetPosition() * (xp*zp);
		}
	}

	if (zp > 0)
	{
		m_particles[x * 2 + z + 1].SetMass(BASE_MASS + EXTRA_MASS*(1 - xp)*zp);
		massDisplayPos += m_particles[x * 2 + z + 1].GetPosition() * ((1 - xp)*zp);
	}
}
	
const char* BridgeDemo::GetTitle()
{
	return "Bombast Physics Bridge Demo";
}

void BridgeDemo::Update()
{
	MassAggregateApp::Update();

	UpdateAdditionalMass();
}

void BridgeDemo::Render()
{
	MassAggregateApp::Render();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	for (unsigned i = 0; i < ROD_COUNT; i++)
	{
		bPhysics::BpParticle** particles = rods[i].particle;
		const bPhysics::BpVec3& p0 = particles[0]->GetPosition();
		const bPhysics::BpVec3& p1 = particles[1]->GetPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}

	glColor3f(0, 1, 0);
	for (unsigned i = 0; i < CABLE_COUNT; i++)
	{
		bPhysics::BpParticle** particles = cables[i].particle;
		const bPhysics::BpVec3& p0 = particles[0]->GetPosition();
		const bPhysics::BpVec3& p1 = particles[1]->GetPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}

	glColor3f(0.7f, 0.7f, 0.7f);
	for (unsigned i = 0; i < SUPPORT_COUNT; i++)
	{
		const bPhysics::BpVec3& p0 = supports[i].particle->GetPosition();
		const bPhysics::BpVec3& p1 = supports[i].anchor;
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}
	glEnd();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(massDisplayPos.x, massDisplayPos.y, massDisplayPos.z);
	glutSolidSphere(0.25f, 20, 10);
	glPopMatrix();
}

void BridgeDemo::KeyEvent(int key, int scanCode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_S:
		massPos.z += 0.1f;
		if (massPos.z > 1.0f) { massPos.z = 1.0f; }
		break;
	case GLFW_KEY_W:
		massPos.z -= 0.1f;
		if (massPos.z < 0.0f) { massPos.z = 0.0f; }
		break;
	case GLFW_KEY_A:
		massPos.x -= 0.1f;
		if (massPos.x < 0) { massPos.x = 0.0f; }
		break;
	case GLFW_KEY_D:
		massPos.x += 0.1f;
		if (massPos.x > 5.0f) { massPos.x = 5.0f; }
		break;

	default:
		MassAggregateApp::KeyEvent(key, scanCode, action, mods);
	}
}

App* GetApp()
{
	return new BridgeDemo();
}