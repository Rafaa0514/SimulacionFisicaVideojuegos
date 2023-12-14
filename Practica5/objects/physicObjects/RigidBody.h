#pragma once
#include "PhysicActor.h"

using namespace physx;

class RigidBody : public PhysicActor {
protected:
	PxRigidStatic* bStatic = nullptr;
	PxRigidDynamic* bDynamic = nullptr;
	bool movable;
	PxShape* shape;
	Vector3 scale;
	Vector4 color;

public:
	RigidBody(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 s, Vector4 col, bool mov, double m, float lt = -1, BoundingBox* _bb = nullptr);

	bool isStatic() { return !movable; }

	// Metodos abstractos
	virtual void addForce(Vector3 f);
	virtual void clearForce();
	virtual Vector3 getVelocity();
	virtual float getHeight();
	void releaseRender() { renderItem->release(); renderItem = nullptr; }

	virtual PhysicActor* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 vel, float lt, BoundingBox* _bb);
};