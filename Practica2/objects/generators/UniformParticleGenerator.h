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
	UniformParticleGenerator(string n, Particle* model, Vector3 var, float pro, bool up = true);
	virtual ~UniformParticleGenerator();

	// Metodos abstractos
	virtual list<Particle*> generateParticles();
	virtual Vector3 calculateVel();
	virtual Vector3 calculatePos();

	// Asignar valor a los intervalos
	void assignVel();
	void assignPos();
};