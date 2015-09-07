#ifndef APP_H
#define APP_H

#include "Timer.h"
#include <BpPhysicsApi.h>

class App
{
public:
	virtual const char* GetTitle();

	virtual void Initialize();
	virtual void Shutdown();

	virtual void SetCamera();

	virtual void Render();
	virtual void Update();

	virtual void KeyEvent(int key, int scanCode, int action, int mods);
	virtual void CharEvent(unsigned int code);
	virtual void MouseClick(int button, int action, int mods);
	virtual void MouseDrag(int x, int y);

	void RenderText(float x, float y, const char* text);

	virtual void WindowResized(int width, int height);

protected:
	int m_width, m_height; 

	Timer* m_pTimer;
};

class MassAggregateApp : public App
{
public:
	MassAggregateApp(unsigned int particleCount);
	virtual ~MassAggregateApp();

	//Update Particle Positions
	virtual void Update();
	virtual void Initialize();
	virtual void Render();

protected:
	bPhysics::BpParticleWorld m_world;
	bPhysics::BpParticle* m_particles;
	bPhysics::BpParticleGroundContacts m_groundContactGenerator;
};

#endif