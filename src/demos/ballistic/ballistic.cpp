#include "gl_headers.h"
#include "App.h"
#include "graphics/Cubes.h"
#include <gl\glut.h>
#include <BpPhysicsApi.h>

class BallisticDemo : public App
{
	enum ProjectileType
	{
		PT_INVALID = 0,
		PT_PISTOL,
		PT_LASER,
		PT_ARTILLERY,
		PT_FIREBALL
	};

	struct Projectile
	{
		bPhysics::BpParticle particle;
		unsigned startTime;
		ProjectileType type;

		void Render()
		{
			bPhysics::BpVec3 position = particle.GetPosition();

			//Projectile
			glColor3f(0, 0, 0);
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glutSolidSphere(0.3f, 5, 4);
			glPopMatrix();

			//Shadow
			glColor3f(0.75f, 0.75f, 0.75f);
			glPushMatrix();
			glTranslatef(position.x, 0, position.z);
			glScalef(1.0f, 0.1f, 1.0f);
			glutSolidSphere(0.6f, 5, 4);
			glPopMatrix();
		}
	};

	const static unsigned MaxProjectiles = 32;
	Projectile projectiles[MaxProjectiles];

	ProjectileType currentProjectileType;

	void Fire();

public:
	BallisticDemo();

	virtual const char* GetTitle();

	virtual void Update();
	virtual void Render();

	virtual void MouseClick(int button, int action, int mods);
	virtual void KeyEvent(int key, int scanCode, int action, int mods);

};

BallisticDemo::BallisticDemo()
	:currentProjectileType(PT_PISTOL)
{
	for (Projectile* projectile = projectiles; projectile < projectiles + MaxProjectiles; projectile++)
	{
		projectile->type = PT_INVALID;
	}
}

const char* BallisticDemo::GetTitle()
{
	return "Bombast Physics Ballistics Demo";
}

void BallisticDemo::Fire()
{
	Projectile* projectile;

	//Find first available projectile slot
	for (projectile = projectiles; projectile < projectile + MaxProjectiles; projectile++)
	{
		if (projectile->type == PT_INVALID) break;
	}

	//No free slot was found so abort
	if (projectile >= projectiles + MaxProjectiles)
	{
		return;
	}

	switch (currentProjectileType)
	{
	case PT_PISTOL:
		projectile->particle.SetMass(2.0f);
		projectile->particle.SetVelocity(0.0f, 0.0f, 35.0f);
		projectile->particle.SetGravity(0.0f, -1.0f, 0.0f);
		projectile->particle.SetDamping(0.001f);
		break;
	case PT_ARTILLERY:
		projectile->particle.SetMass(2.0f);
		projectile->particle.SetVelocity(0.0f, 30.0f, 40.0f);
		projectile->particle.SetGravity(0.0f, -20.0f, 0.0f);
		projectile->particle.SetDamping(0.001f);
		break;
	case PT_LASER:
		projectile->particle.SetMass(0.1f);
		projectile->particle.SetVelocity(0.0f, 0.0f, 10.0f);
		projectile->particle.SetGravity(0.0f, 0.0f, 0.0f);
		projectile->particle.SetDamping(0.001f);
		break;
	case PT_FIREBALL:
		projectile->particle.SetMass(1.0f);
		projectile->particle.SetVelocity(0.0f, 0.0f, 10.0f);
		projectile->particle.SetGravity(0.0f, 0.6f, 0.0f);
		projectile->particle.SetDamping(0.001f);
		break;
	}

	projectile->particle.SetPosition(0.0f, 1.5f, 0.0f);
	projectile->startTime = m_pTimer->GetTime();
	projectile->type = currentProjectileType;

	projectile->particle.ClearAccumulator();
}

void BallisticDemo::Update()
{
	App::Update();

	float duration = (float)m_pTimer->GetFrameTime() * 0.001f;
	if (duration <= 0.0f) return;

	for (Projectile* projectile = projectiles; projectile < projectiles + MaxProjectiles; projectile++)
	{
		if (projectile->type != PT_INVALID)
		{
			projectile->particle.Simulate(m_pTimer->GetTime() * 0.001f, duration);

			if (projectile->particle.GetPosition().y < 0.0f || projectile->startTime+5000 < m_pTimer->GetTime() || projectile->particle.GetPosition().z > 200.0f)
			{
				projectile->type = PT_INVALID;
			}
		}
	}
}

void BallisticDemo::Render()
{
	//Clear viewport & set camera direction
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

	for (Projectile* projectile = projectiles; projectile < projectiles + MaxProjectiles; projectile++)
	{
		if (projectile->type != PT_INVALID)
		{
			projectile->Render();
		}
	}

	//Render Description
	glColor3f(0.0f, 0.0f, 0.0f);
	RenderText(10.0f, 34.0f, "Click: Fire\n1-4: Change Ammo");

	switch (currentProjectileType)
	{
	case PT_PISTOL: RenderText(10.0f, 10.0f, "Current Ammo: Pistol"); break;
	case PT_ARTILLERY: RenderText(10.0f, 10.0f, "Current Ammo: Artillery"); break;
	case PT_FIREBALL: RenderText(10.0f, 10.0f, "Current Ammo: Fireball"); break;
	case PT_LASER: RenderText(10.0f, 10.0f, "Current Ammo: Laser"); break;
	}
}

void BallisticDemo::MouseClick(int button, int action, int mods)
{
	if (action == GLFW_PRESS) { Fire(); }
}

void BallisticDemo::KeyEvent(int key, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key) {
		case GLFW_KEY_1:
			currentProjectileType = PT_PISTOL;
			break;
		case GLFW_KEY_2:
			currentProjectileType = PT_LASER;
			break;
		case GLFW_KEY_3:
			currentProjectileType = PT_FIREBALL;
			break;
		case GLFW_KEY_4:
			currentProjectileType = PT_ARTILLERY;
			break;
		}
	}
}

App* GetApp()
{
	return new BallisticDemo();
}