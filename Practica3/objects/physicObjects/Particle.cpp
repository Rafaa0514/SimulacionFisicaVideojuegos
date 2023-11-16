#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 v, float rad, double m, Vector4 color, double lt, BoundingBox* _bb, float dp) :
	pose(pos), vel(v), acceleration({ 0,0,0 }), force({ 0,0,0 }), damping(dp),
	lifeTime(lt), radious(rad),
	renderItem(new RenderItem(CreateShape(physx::PxSphereGeometry(rad)),
		&pose, color)), bb(_bb) {
	setMass(m);
}

Particle::~Particle() {
	renderItem->release();
	renderItem = nullptr;
}

bool Particle::integrate(double t) {
	lifeTime -= t;
	if (fabs(inv_mass) > 1e-10) {
		acceleration = force * inv_mass;
		vel += acceleration * t;

		vel *= powf(damping, t);

		pose.p += vel * t;

		clearForce();
	}

	return (lifeTime >= 0 && bb->isInside(pose.p));
}

void Particle::setMass(double m) {
	if (m <= 0.0) inv_mass = 0;
	else inv_mass = 1.0f / m;

	mass = m;
}

Particle* Particle::clone() const {
	return new Particle(pose.p, vel, radious, mass, renderItem->color, lifeTime, bb);
}

Particle* Particle::clone(Vector3 p, Vector3 v) const {
	return new Particle(p, v, radious, mass, renderItem->color, lifeTime, bb);
}

void Particle::clearForce() {
	force *= 0;
}

void Particle::addForce(const Vector3& f) {
	force += f;
}