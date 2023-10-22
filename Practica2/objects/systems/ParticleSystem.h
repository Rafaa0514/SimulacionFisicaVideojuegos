#pragma once
#include "../generators/UniformParticleGenerator.h"
#include <list>
#include <vector>


// enum Colors { RED, BLUE, YELLOW, GREEN };
// 
// vector<Vector4> colores = {
// 	Vector4(255, 0, 0, 1),
// 	Vector4(0, 255, 255, 1),
// 	Vector4(255, 255, 0, 1),
// 	Vector4(0, 255, 0, 1)
// };

class ParticleSystem {
protected:
	std::list<Particle*> myParticles;
	std::list<ParticleGenerator*> particleGenerators;
	std::vector<list<Particle*>::iterator> deadParticles;

public:
	ParticleSystem();
	~ParticleSystem();

	void update(double t);

	ParticleGenerator* getParticleGenerator(string name);

	void generateFireworkSystem();
};