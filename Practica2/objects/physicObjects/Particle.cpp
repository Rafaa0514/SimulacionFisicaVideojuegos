#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 v, Vector3 acce, float dp) :
	pose(pos), vel(v), acceleration(acce), damping(dp), 
	lifeTime(5.0f), startTime(GetLastTime()),
	renderItem(new RenderItem(CreateShape(physx::PxSphereGeometry(1)),
		&pose, Vector4(255, 255, 255, 1))) {

}

Particle::~Particle() {
	renderItem->release();
	renderItem = nullptr;
}

bool Particle::integrate(double t) {
	if (startTime + lifeTime < GetLastTime()) {
		pose.p += vel * t;

		vel += acceleration * t;

		vel *= powf(damping, t);

		return true;
	}
	return false;
}

Particle* Particle::clone() const {
	return new Particle(pose.p, vel, acceleration);
}