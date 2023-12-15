#include "PhysicActor.h"

PhysicActor::PhysicActor(Vector3 pos, double m, float lt, BoundingBox* _bb, float dp) :
	pose(pos), lifeTime(lt), damping(dp), bb(_bb), renderItem(nullptr) {
	setMass(m);
}

PhysicActor::~PhysicActor() {
	releaseRender();
}

void PhysicActor::setMass(double m) {
	if (m <= 0.0) inv_mass = 0;
	else inv_mass = 1.0f / m;

	mass = m;
}


bool PhysicActor::integrate(double t) {
	if (lifeTime != -1) lifeTime -= t;
	return ((bb == nullptr || bb->isInside(pose.p)) && (lifeTime == -1 || lifeTime > 0));
}


void PhysicActor::releaseRender(){ 
	if (renderItem != nullptr) {
		renderItem->release();
		renderItem = nullptr;
	}
}