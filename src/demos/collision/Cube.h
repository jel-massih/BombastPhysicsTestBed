#pragma once
#include <BpPhysicsApi.h>
#include "vectorial\vectorial.h"

struct Cube
{
	bPhysics::BpParticle particle;

	Cube()
	{
		particle.SetPosition(0, 5, 0);
		particle.SetMass(1);
		particle.SetGravity(0, -9, 0);
	}

	void Render(const vectorial::vec3f& light, float alpha = 1.0f)
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

		//Render shadow volume
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		glEnable(GL_STENCIL_TEST);

		vectorial::vec3f bodySpaceLight = vectorial::vec3f(position.x, position.y, position.z);
		
		glStencilFunc(GL_ALWAYS, 0x0, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

		renderShadowVolume(particle.GetState(), bodySpaceLight);

		glCullFace(GL_FRONT);
		glStencilFunc(GL_ALWAYS, 0x0, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		
		renderShadowVolume(particle.GetState(), bodySpaceLight);

		glCullFace(GL_BACK);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);

		glDisable(GL_STENCIL_TEST);

		glPopMatrix();
	}

	static void renderShadowVolume(const bPhysics::BpParticle::IntegrationState &state, const vectorial::vec3f &light)
	{
		glBegin(GL_QUADS);

		const float s = 1.0f * 0.5f;

		silhouette(light, vectorial::vec3f(-s, +s, -s), vectorial::vec3f(+s, +s, -s));
		silhouette(light, vectorial::vec3f(+s, +s, -s), vectorial::vec3f(+s, +s, +s));
		silhouette(light, vectorial::vec3f(+s, +s, +s), vectorial::vec3f(-s, +s, +s));
		silhouette(light, vectorial::vec3f(-s, +s, +s), vectorial::vec3f(-s, +s, -s));

		silhouette(light, vectorial::vec3f(-s, -s, -s), vectorial::vec3f(+s, -s, -s));
		silhouette(light, vectorial::vec3f(+s, -s, -s), vectorial::vec3f(+s, -s, +s));
		silhouette(light, vectorial::vec3f(+s, -s, +s), vectorial::vec3f(-s, -s, +s));
		silhouette(light, vectorial::vec3f(-s, -s, +s), vectorial::vec3f(-s, -s, -s));

		silhouette(light, vectorial::vec3f(-s, +s, -s), vectorial::vec3f(-s, -s, -s));
		silhouette(light, vectorial::vec3f(+s, +s, -s), vectorial::vec3f(+s, -s, -s));
		silhouette(light, vectorial::vec3f(+s, +s, +s), vectorial::vec3f(+s, -s, +s));
		silhouette(light, vectorial::vec3f(-s, +s, +s), vectorial::vec3f(-s, -s, +s));

		glEnd();
	}

	static float vectorDot(const vectorial::vec3f& a, const vectorial::vec3f& b)
	{
		return (a.x() * b.x()) + (a.y() * b.y()) + (a.z() * b.z());
	}

	static vectorial::vec3f vectorCross(const vectorial::vec3f& a, const vectorial::vec3f& b)
	{
		return vectorial::vec3f(
				a.y() * b.z() - a.z() * b.y(),
				a.z() * b.x() - a.x() * b.z(),
				a.x() * b.y() - a.y() * b.x()
			);
	}

	static void silhouette(const vectorial::vec3f &light, vectorial::vec3f& a, vectorial::vec3f& b)
	{
		// determine edge normals

		vectorial::vec3f midpoint = (a + b) * 0.5f;

		vectorial::vec3f leftNormal;

		if (midpoint.x() != 0)
			leftNormal = vectorial::vec3f(midpoint.x(), 0, 0);
		else
			leftNormal = vectorial::vec3f(0, midpoint.y(), 0);

		vectorial::vec3f rightNormal = midpoint - leftNormal;

		// check if silhouette edge

		const vectorial::vec3f differenceA = a - light;

		const float leftDot = vectorDot(leftNormal, differenceA);
		const float rightDot = vectorDot(rightNormal, differenceA);

		if ((leftDot < 0 && rightDot>0) || (leftDot > 0 && rightDot < 0))
		{
			// extrude quad

			const vectorial::vec3f differenceB = b - light;

			vectorial::vec3f _a = a + differenceA * 100;
			vectorial::vec3f _b = b + differenceB * 100;

			// ensure correct winding order for silhouette edge

			const vectorial::vec3f cross = vectorCross(b - a, differenceA);

			if (vectorDot(cross, a) < 0)
			{
				vectorial::vec3f t = a;
				a = b;
				b = t;

				t = _a;
				_a = _b;
				_b = t;
			}

			// render extruded quad

			glVertex3f(a.x(), a.y(), a.z());
			glVertex3f(b.x(), b.y(), b.z());
			glVertex3f(_b.x(), _b.y(), _b.z());
			glVertex3f(_a.x(), _a.y(), _a.z());
		}
	}
};