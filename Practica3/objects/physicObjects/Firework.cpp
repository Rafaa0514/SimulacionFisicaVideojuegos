#include "Firework.h"

Firework::Firework(Vector3 p, Vector3 v, double m, Vector4 color, float lt, BoundingBox* bb, int g, int n) : 
	Particle(p, v, FIREWORK_RAD, m, color, lt, bb), gen(g), nSons(n), maxLife(lt) {}

Firework::~Firework() {

}

Firework* Firework::clone(Vector3 p, Vector3 v, float lt, int s) const {
	return new Firework(p, v, mass, renderItem->color, lt, bb, gen - 1, s);
}