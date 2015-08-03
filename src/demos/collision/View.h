#pragma once
#include "Client.h"
#include "vectorial\vectorial.h"

struct View
{
	View()
	{

	}

	void Initialize(Client& client)
	{
		client.Initialize();

		this->client = &client;

		glEnable(GL_LIGHT0);
		glShadeModel(GL_SMOOTH);

		GLfloat lightAmbientColor[] = { 0.5, 0.5, 0.5, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbientColor);

		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);

		GLfloat lightPosition[] = { 25.0, 100.0, 25.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		light = vectorial::vec3f(lightPosition[0], lightPosition[1], lightPosition[2]);

		renderClient = true;
	}

	void Update(float dt)
	{

	}

	void Render(float alpha = 1.0f)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		const float d = 100;
		const float h = -0.025f;

		glBegin(GL_QUADS);
		glColor3f(60.0f / 255.0f, 99.0f / 255.0f, 123.0f / 255.0f);
		glVertex3f(-d, h, +d);
		glVertex3f(+d, h, +d);
		glVertex3f(+d, h, -d);
		glVertex3f(-d, h, -d);
		glEnd();

		if (renderClient)
			client->cube.Render(light, alpha);

		glEnable(GL_STENCIL_TEST);
		
		glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

		glColor4f(0.25f, 0.25f, 0.25f, 0.5f);

		const float s = 10;
		glVertex3f(-s, -s, 7);
		glVertex3f(+s, -s, 7);
		glVertex3f(+s, +s, 7);
		glVertex3f(-s, +s, 7);

		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_STENCIL_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_BLEND);
	}

	bool renderClient;

private:
	Client* client;

	vectorial::vec3f light;
};