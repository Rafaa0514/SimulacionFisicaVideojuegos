#include "Particle.h"


Particle::Particle(Vector3 pos, Vector3 v, Vector3 acce, float dp) :
	pose(pos), vel(v), acceleration(acce), damping(dp),
	renderItem(new RenderItem(CreateShape(physx::PxSphereGeometry(1)),
		&pose, Vector4(255, 255, 255, 1))) {

}

Particle::~Particle() {
	renderItem->release();
	renderItem = nullptr;
}

void Particle::integrate(double t) {
	pose.p += vel * t;

	vel += acceleration * t;

	vel *= powf(damping, t);
}