#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 v, float rad, double m, Vector4 col, double lt, BoundingBox* _bb, float dp) : 
	PhysicActor(pos, m, lt, _bb, dp), vel(v), radious(rad), color(col), force(Vector3(0)), acc(Vector3(0)) {
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(radious)), &pose, color);
}

bool Particle::integrate(double t) {
	if (fabs(inv_mass) > 1e-10) {
		acc = force * inv_mass;
		vel += acc * t;

		vel *= powf(damping, t);

		pose.p += vel * t;

		clearForce();
	}

	return PhysicActor::integrate(t);
}


bool Particle::collides(PhysicActor* other) {
	return abs((other->getPos() - getPos()).magnitude()) < (getRadious() + other->getRadious());
}

void Particle::addForce(Vector3 f) {
	force += f;
}

void Particle::clearForce() {
	force *= 0;
}

Vector3 Particle::getVelocity() {
	return vel;
}

float Particle::getHeight() {
	return radious * 2;
}

PhysicActor* Particle::clone(PxPhysics* gPx, PxScene* scene, Vector3 p, Vector3 v, float lt, BoundingBox* _bb) {
	return new Particle(p, v, radious, mass, color, lt, _bb);
}


PhysicActor* Particle::clone(PxPhysics* gPx, PxScene* scene, Vector3 p, Vector3 v, float lt, BoundingBox* _bb, double rnd) {
	return new Particle(p, v, radious * rnd, mass * rnd, color, lt * rnd, _bb);
}

Particle* Particle::clone() {
	return new Particle(pose.p, vel, radious, mass, color, lifeTime, bb);
}