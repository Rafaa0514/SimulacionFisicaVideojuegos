#include "Forces.h"

bool GravityGenerator::updateForce(PhysicActor* p) {
	if (fabs(p->getInvMass()) < 1e-10 || !bb->isInside(p->getPos())) return false;

	p->addForce(grav * p->getMass());
	return true;
}

bool WindGenerator::updateForce(PhysicActor* p) {
	if (fabs(p->getInvMass()) < 1e-10 || !bb->isInside(p->getPos())) return false;

	Vector3 diffVel = wVel - p->getVelocity();

	Vector3 resultantForce = k1 * diffVel + k2 * diffVel.magnitude() * diffVel;
	p->addForce(resultantForce);
	return true;
}

bool TornadoGenerator::updateForce(PhysicActor* p) {
	if (fabs(p->getInvMass()) < 1e-10 || !bb->isInside(p->getPos())) return false;

	Vector3 diff = p->getPos() - origin;
	wVel = k * Vector3(-diff.z, 50 - diff.y, diff.x);

	WindGenerator::updateForce(p);
	return true;
}

bool ExplosionGenerator::updateForce(PhysicActor* p) {
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


bool AnchoredSpringGenerator::updateForce(PhysicActor* p) {
	Vector3 distance = point - p->getPos();
	double l = distance.normalize();
	float deltaX = l - r_length;
	Vector3 resultingForce = distance * deltaX * k;
	p->addForce(resultingForce);
	return true;
}

bool SpringGenerator::updateForce(PhysicActor* p) {
	if (elastic) {
		double distance = (p->getPos() - other->getPos()).magnitude();
		if (distance < r_length) {  return true; }
	}
	point = other->getPos();
	return AnchoredSpringGenerator::updateForce(p);
}

bool BouyancyForceGenerator::updateForce(PhysicActor* p) {
	if (!bb->isInside(p->getPos())) return false;

	float h = p->getPos().y;
	float h0 = fluidLimit->transform->p.y;
	Vector3 f(0, 0, 0);
	float immersed = 0.0;

	if (h - h0 > height * 0.5) immersed = 0.0;
	else if (h0 - h > height * 0.5) immersed = 1.0;
	else immersed = (h0 - h) / height + 0.5;
	volume = pow(p->getHeight(), 3);
	f.y = liquid_density * volume * immersed * -GLOBAL_GRAVITY.y;
	p->addForce(f);
	return true;
}