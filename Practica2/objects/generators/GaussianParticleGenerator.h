#pragma once
#include "ParticleGenerator.h"

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	normal_distribution<float>* velX;
	normal_distribution<float>* velY;
	normal_distribution<float>* velZ;

	normal_distribution<float>* posX;
	normal_distribution<float>* posY;
	normal_distribution<float>* posZ;

public:
	GaussianParticleGenerator(string n, Particle* model, Vector3 vel, float prob, bool up = true, Vector3 var_pos = Vector3(1));
	virtual ~GaussianParticleGenerator();

	// Metodos abstractos
	virtual list<Particle*> generateParticles();
	virtual Vector3 calculateVel();
	virtual Vector3 calculatePos();

	// Asignar valor a los intervalos
	void assignVel();
	void assignPos();
};