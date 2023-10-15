#pragma once
#include "ParticleGenerator.h"

class UniformParticleGenerator : public ParticleGenerator {
protected:
	Vector3 vel_width, pos_witdh;

public:
	UniformParticleGenerator(Vector3 pos, Vector3 vel);
	virtual list<Particle*> generateParticles();
};