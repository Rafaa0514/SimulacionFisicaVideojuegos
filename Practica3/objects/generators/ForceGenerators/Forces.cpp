#include "Forces.h"

bool GravityGenerator::updateForce(Particle* p) {
	if (fabs(p->getInvMass()) < 1e-10 || !bb->isInside(p->getPos())) return false;

	p->addForce(grav * p->getMass());
	return true;
}

bool WindGenerator::updateForce(Particle* p) {
	if (fabs(p->getInvMass()) < 1e-10 || !bb->isInside(p->getPos())) return false;

	Vector3 diffVel = wVel - p->getVel();

	Vector3 resultantForce = k1 * diffVel + k2 * diffVel.magnitude() * diffVel;
	p->addForce(resultantForce);
	return true;
}

bool TornadoGenerator::updateForce(Particle* p) {
	if (fabs(p->getInvMass()) < 1e-10 || !bb->isInside(p->getPos())) return false;

	Vector3 diff = p->getPos() - origin;
	wVel = k * Vector3(-diff.z, 50 - diff.y, diff.x);

	WindGenerator::updateForce(p);
	return true;
}

bool ExplosionGenerator::updateForce(Particle* p) {
	if (fabs(p->getInvMass()) < 1e-10) return false;

	Vector3 dif = p->getPos() - center;
	double dist = dif.magnitude();
	if (dist < 1e-10) dist = 0.1;
	rad = (finalRad / duration) * time;

	if (dist < rad) {
		Vector3 resultantForce = k * dif / (dist * dist);
		resultantForce *= exp(-time / tau);
		p->addForce(resultantForce);
	}
	if (dist > finalRad) return false;
	return true;
}