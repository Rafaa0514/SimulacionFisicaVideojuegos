#include "RigidBody.h"

RigidBody::RigidBody(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 s, Vector3 vel, Vector4 col, bool mov, double m, BoundingBox* _bb, float lt) :
	PhysicActor(pos, m, lt, _bb), scale(s), color(col), movable(mov) {
	shape = CreateShape(PxBoxGeometry(scale));

	if (movable) {
		bDynamic = gPx->createRigidDynamic(pose);
		bDynamic->attachShape(*shape);
		renderItem = new RenderItem(shape, bDynamic, color);
		scene->addActor(*bDynamic);

		bDynamic->setLinearVelocity(vel);
		density = m / ((scale.x * 2) * (scale.y * 2) * (scale.z * 2));
		PxRigidBodyExt::setMassAndUpdateInertia(*static_cast<PxRigidBody*>(bDynamic), density);
	}
	else {
		bStatic = gPx->createRigidStatic(pose);
		bStatic->attachShape(*shape);
		renderItem = new RenderItem(shape, bStatic, color);
		scene->addActor(*bStatic);
	}
}

RigidBody::~RigidBody() {
	if (movable) bDynamic->release();
	else bStatic->release();

	renderItem->release();
	renderItem = nullptr;
}

bool RigidBody::integrate(double t) {
	if (movable) {
		Vector3 vel = bDynamic->getLinearVelocity();
		vel *= powf(damping, t);

		pose.p += vel * t;
	}
	return PhysicActor::integrate(t);
}

bool RigidBody::collides(PhysicActor* other) {
	return abs((other->getPos() - getPos()).magnitude()) < getRadious();
}

void RigidBody::addForce(Vector3 f) {
	if (movable) bDynamic->addForce(f);
}

void RigidBody::clearForce() {
	if (movable) bDynamic->clearForce();
}

Vector3 RigidBody::getVelocity() {
	if (movable) return bDynamic->getLinearVelocity();
	return Vector3(0);
}

float RigidBody::getHeight() {
	return scale.y * 2;
}

float RigidBody::getRadious() {
	float rad = 0;
	rad = max(rad, scale.x);
	rad = max(rad, scale.y);
	rad = max(rad, scale.z);
	return rad;
}

PhysicActor* RigidBody::clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 vel, float lt, BoundingBox* _bb) {
	return new RigidBody(gPx, scene, pos, scale, vel, color, movable, mass, _bb, lt);
}