#pragma once
#include <BpPhysicsApi.h>

struct Cube
{
	bPhysics::BpParticle particle;

	void Render()
	{
		bPhysics::BpVec3 position = particle.GetPosition();

		glPushMatrix();

		glTranslatef(position.x, position.y, position.z);

		GLfloat color[] = { 1, 1, 1, 1 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

		glEnable(GL_LIGHTING);

		glDepthFunc(GL_ALWAYS);

		const float s = 1 * 0.5f;

		glBegin(GL_QUADS);

		glNormal3f(0, 0, 1);
		glVertex3f(-s, -s, s);
		glVertex3f(s, -s, s);
		glVertex3f(s, s, s);
		glVertex3f(-s, s, s);

		glNormal3f(0, 0, -1);
		glVertex3f(-s, -s, -s);
		glVertex3f(-s, s, -s);
		glVertex3f(s, s, -s);
		glVertex3f(s, -s, -s);

		glNormal3f(0, 1, 0);
		glVertex3f(-s, s, -s);
		glVertex3f(-s, s, s);
		glVertex3f(s, s, s);
		glVertex3f(s, s, -s);

		glNormal3f(0, -1, 0);
		glVertex3f(-s, -s, -s);
		glVertex3f(s, -s, -s);
		glVertex3f(s, -s, s);
		glVertex3f(-s, -s, s);

		glNormal3f(1, 0, 0);
		glVertex3f(s, -s, -s);
		glVertex3f(s, s, -s);
		glVertex3f(s, s, s);
		glVertex3f(s, -s, s);

		glNormal3f(-1, 0, 0);
		glVertex3f(-s, -s, -s);
		glVertex3f(-s, -s, s);
		glVertex3f(-s, s, s);
		glVertex3f(-s, s, -s);

		glEnd();

		glDepthFunc(GL_LEQUAL);

		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		//Render shadow

		glPopMatrix();
	}
};