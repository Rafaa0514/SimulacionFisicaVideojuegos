#pragma once
#include <string>
#include <list>
#include "../physicObjects/Particle.h"
using namespace std;

class ParticleGenerator {
protected:
	string name;
	Vector3 mean_pos, mean_vel;
	double generation_probability;
	int num_particles;
	Particle* model;

public:
	ParticleGenerator(Vector3 pos, Vector3 vel);
	void setParticle(Particle* model);
	virtual list<Particle*> generateParticles() = 0;
};