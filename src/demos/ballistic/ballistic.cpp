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
		PT_PISTOL
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

	void Fire();

public:
	BallisticDemo();

	virtual const char* GetTitle();

	virtual void Update();
	virtual void Render();

	virtual void Mouse(int button, int state, int x, int y);
	virtual void Key(unsigned char key);
};

BallisticDemo::BallisticDemo()
{

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
	
	projectile->particle.SetMass(2.0f);
	projectile->particle.SetVelocity(0.0f, 0.0f, 35.0f);
	projectile->particle.SetAcceleration(0.0f, 0.0f, 35.0f);
	projectile->particle.SetDamping(0.99f);

	projectile->particle.SetPosition(0.0f, 1.5f, 0.0f);
	projectile->type = PT_PISTOL;

	projectile->particle.ClearAccumulator();
}

void BallisticDemo::Update()
{
	for (Projectile* projectile = projectiles; projectile < projectiles + MaxProjectiles; projectile++)
	{
		if (projectile->type != PT_INVALID)
		{
			projectile->particle.Simulate(duration);

			if (projectile->particle.GetPosition().y < 0.0f || projectile->particle.GetPosition().z > 200f)
			{
				projectile->type = PT_INVALID;
			}
		}
	}

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

void BallisticDemo::Mouse(int button, int state, int x, int y)
{

}

void BallisticDemo::Key(unsigned char key)
{

}

App* GetApp()
{
	return new BallisticDemo();
}