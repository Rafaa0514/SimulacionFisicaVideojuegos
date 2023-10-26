#pragma once
#include "Particle.h"
#include <list>

using namespace std;

const float FIREWORK_RAD = 3;

class Firework : public Particle {
protected:
	int gen, nSons;
	string myGenerator;
	float maxLife;

public:
	Firework(Vector3 p, Vector3 v, Vector3 a, Vector4 color, float lt, int g, int n);
	virtual ~Firework();

	int getGeneration() { return gen; }
	int getNumberSons() { return nSons; }
	float getLifetime() { return maxLife; }

	virtual Firework* clone(Vector3 p, Vector3 v, float lt, int s) const;
};