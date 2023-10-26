#include "Firework.h"

Firework::Firework(Vector3 p, Vector3 v, Vector3 a, Vector4 color, float lt, int g, int n) : 
	Particle(p,v,a,FIREWORK_RAD, color, lt), gen(g), nSons(n), maxLife(lt) {}

Firework::~Firework() {

}

Firework* Firework::clone(Vector3 p, Vector3 v, float lt, int s) const {
	return new Firework(p, v, Vector3(0,-10,0), renderItem->color, lt, gen - 1, s);
}