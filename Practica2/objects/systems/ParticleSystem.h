#pragma once
#include "../physicObjects/Particle.h"
#include "../generators/ParticleGenerator.h"
#include <list>
#include <vector>

enum Colors { RED, BLUE, YELLOW, GREEN };
vector<Vector4> colores = {
	Vector4(253, 0, 0, 1),
	Vector4(0, 114, 224, 1),
	Vector4(255, 245, 0, 1),
	Vector4(91, 244, 0, 1)
};

class ParticleSystem {
protected:
	std::list<Particle*> myParticles;
	std::list<ParticleGenerator*> particleGenerator;
	std::vector<list<Particle*>::iterator> deadParticles;

public:
	ParticleSystem();

	void update(double t);

	ParticleGenerator* getParticleGenerator(string name);

	void generateFireworkSystem();
};