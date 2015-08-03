#pragma once
#include "Scene.h"

struct Client : public Scene
{
	Client()
	{
	}

	void Update(float dt)
	{
		Scene::Update(dt);
	}

	void Synchronize(unsigned int t, const bPhysics::BpParticle::IntegrationState& state)
	{
		bPhysics::BpParticle::IntegrationState original = cube.particle.GetState();
	}
};