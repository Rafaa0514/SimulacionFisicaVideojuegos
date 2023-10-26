#pragma once
#include "../generators/UniformParticleGenerator.h"
#include "../generators/GaussianParticleGenerator.h"
#include "../generators/FireworkGenerator.h"
#include <list>
#include <vector>


class ParticleSystem {
protected:
	std::list<Particle*> myParticles;
	std::list<ParticleGenerator*> particleGenerators;
	std::vector<list<Particle*>::iterator> deadParticles;

	std::list<Firework*> fireworks_pool;
	FireworkGenerator* fireworkGen;
	std::vector<list<Firework*>::iterator> deadFireworks;

public:
	ParticleSystem();
	~ParticleSystem();

	void update(double t);

	ParticleGenerator* getParticleGenerator(string name);

	void generateFireworkSystem();
};