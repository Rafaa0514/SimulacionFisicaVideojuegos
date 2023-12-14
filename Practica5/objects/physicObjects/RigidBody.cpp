#include "RigidBody.h"

RigidBody::RigidBody(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 s, Vector4 col, bool mov, double m, float lt, BoundingBox* _bb) : 
	PhysicActor(pos, m, lt, _bb), scale(s), color(col), movable(mov) {
	shape = CreateShape(PxBoxGeometry(scale));

	if (movable) {
		bDynamic = gPx->createRigidDynamic(pose);
		bDynamic->attachShape(*shape);
		renderItem = new RenderItem(shape, bDynamic, color);
		scene->addActor(*bDynamic);
	}
	else {
		bStatic = gPx->createRigidStatic(pose);
		bStatic->attachShape(*shape);
		renderItem = new RenderItem(shape, bStatic, color);
		scene->addActor(*bStatic);
	}
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

PhysicActor* RigidBody::clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 vel, float lt, BoundingBox* _bb) {
	return new RigidBody(gPx, scene, pos, vel, color, movable, mass, lt, _bb);
}