#pragma once

#include "Cube.h"
#include <vectorial/vectorial.h>
#include <gl/glew.h>
#include "vectorial/mat4f.h"

struct Scene
{
	Scene()
	{
		time = 0;
	}

	~Scene()
	{

	}

	void Initialize()
	{
		bPhysics::BpPlane left, right, top, bottom, front, back;
		CalculateFrustumPlanes(left, right, bottom, top, front, back);

		front.constant += 2.0f;

		bPhysics::BpPlane ramp = bPhysics::BpPlane(bPhysics::BpVec3(0, 1, -1), bPhysics::BpVec3(0, 0, 5));

		planes.push_back(left);
		planes.push_back(right);
		planes.push_back(front);
		planes.push_back(back);
		planes.push_back(ramp);

		bPhysics::BpPlane floor(bPhysics::BpVec3(0, 1, 0), 0);
		planes.push_back(floor);
	}

	void Update(unsigned int t)
	{
		cube.particle.Simulate(t);
	}

public:
	unsigned int time;

	Cube cube;

	std::vector<bPhysics::BpPlane> planes;
};

void CalculateFrustumPlanes(bPhysics::BpPlane& left, bPhysics::BpPlane& right, bPhysics::BpPlane& bottom, bPhysics::BpPlane& top, bPhysics::BpPlane& front, bPhysics::BpPlane& back)
{
	GLfloat projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projection);

	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

	vectorial::mat4f projM = vectorial::mat4f(projection);
	vectorial::mat4f mvM = vectorial::mat4f(modelView);

	vectorial::mat4f clipM = mvM * projM;
	GLfloat clip[16];
	clipM.store(clip);

	left.normal.x = clip[3] + clip[0];
	left.normal.y = clip[7] + clip[4];
	left.normal.z = clip[11] + clip[8];
	left.constant = -(clip[15] + clip[12]);
	left.Normalize();

	right.normal.x = clip[3] - clip[0];
	right.normal.y = clip[7] - clip[4];
	right.normal.z = clip[11] - clip[8];
	right.constant = -(clip[15] - clip[12]);
	right.Normalize();

	bottom.normal.x = clip[3] + clip[1];
	bottom.normal.y = clip[7] + clip[5];
	bottom.normal.z = clip[11] + clip[9];
	bottom.constant = -(clip[15] + clip[13]);
	bottom.Normalize();

	top.normal.x = clip[3] - clip[1];
	top.normal.y = clip[7] - clip[5];
	top.normal.z = clip[11] - clip[9];
	top.constant = -(clip[15] - clip[13]);
	top.Normalize();

	front.normal.x = clip[3] + clip[2];
	front.normal.y = clip[7] + clip[6];
	front.normal.z = clip[11] + clip[10];
	front.constant = -(clip[15] + clip[14]);
	front.Normalize();

	back.normal.x = clip[3] - clip[2];
	back.normal.y = clip[7] - clip[6];
	back.normal.z = clip[11] - clip[10];
	back.constant = -(clip[15] - clip[14]);
	back.Normalize();

}