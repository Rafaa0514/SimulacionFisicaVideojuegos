#pragma once
#include "ParticleGenerator.h"

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 std_dev_pos, std_dev_vel;
	double std_dev_t;

public:
	GaussianParticleGenerator(Vector3 pos, Vector3 vel);
	virtual list<Particle*> generateParticles();
};