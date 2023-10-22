#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 v, float rad, Vector4 color, float lt, float dp) :
	pose(pos), vel(v), acceleration(Vector3(0)), damping(dp), 
	lifeTime(lt), startTime(GetLastTime()), radious(rad),
	renderItem(new RenderItem(CreateShape(physx::PxSphereGeometry(rad)),
		&pose, color)) {

}

Particle::~Particle() {
	renderItem->release();
	renderItem = nullptr;
}

bool Particle::integrate(double t) {
	pose.p += vel * t;

	vel += acceleration * t;

	vel *= powf(damping, t);
	float a = GetLastTime();
	if (startTime + lifeTime < GetLastTime()) return false;
	return true;
}

Particle* Particle::clone() const {
	return new Particle(pose.p, vel, radious, renderItem->color, lifeTime);
}

Particle* Particle::clone(Vector3 p, Vector3 v, float lt) const {
	return new Particle(p, v, radious, renderItem->color, lt);
}