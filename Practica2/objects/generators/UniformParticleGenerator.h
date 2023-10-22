#pragma once
#include "ParticleGenerator.h"

class UniformParticleGenerator : public ParticleGenerator {
protected:

	uniform_real_distribution<float>* velX;
	uniform_real_distribution<float>* velY;
	uniform_real_distribution<float>* velZ;
	uniform_real_distribution<float>* posX;
	uniform_real_distribution<float>* posY;
	uniform_real_distribution<float>* posZ;

public:
	UniformParticleGenerator(Particle* model, Vector3 var, float pro, bool up = true);
	virtual ~UniformParticleGenerator();
	virtual list<Particle*> generateParticles();

	// Asignar valor a los intervalos
	void assignVel();
	void assignPos();
};