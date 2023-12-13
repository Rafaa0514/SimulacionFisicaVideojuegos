#pragma once
#include "../../physicObjects/Particle.h"
#include "../../physicObjects/BoxParticle.h"
#include "../../physicObjects/BoundingBox.h"

using namespace std;

class ForceGenerator {
protected:
	string name;
	double time; // If starting negative --> eternal
	double duration = 0.0;
	BoundingBox* bb;

public:
	ForceGenerator(string n, double d, Vector3 bbC, Vector3 bbS) :
		name(n), time(0.0), duration(d), bb(new BoundingBox(bbC, bbS)) {}

	virtual bool updateForce(Particle* particle) = 0;

	inline bool updateTime(double t) {
		time += t;
		return time < duration || duration < 0.0; // Devuelve true si ya era cero o si es positivo
	}
	virtual ~ForceGenerator() { delete bb; }
};