#pragma once
#include "Particle.h"
#include <list>

using namespace std;

const float FIREWORK_RAD = 3;

class Firework : public Particle {
protected:
	int gen, nSons;
	float maxLife;

public:
	Firework(Vector3 p, Vector3 v, double m, Vector4 color, float lt, BoundingBox* bb, int g, int n);

	int getGeneration() { return gen; }
	int getNumberSons() { return nSons; }
	float getLifetime() { return maxLife; }

	virtual Firework* clone(Vector3 p, Vector3 v, float lt, int s) const;
};