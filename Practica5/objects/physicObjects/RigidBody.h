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
	double density;
	Vector4 color;
public:
	RigidBody(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 s, Vector3 vel, Vector4 col, bool mov, double m, BoundingBox* _bb = nullptr, float lt = -1);

	virtual ~RigidBody();

	bool isStatic() { return !movable; }
	virtual bool integrate(double t);

	// Metodos abstractos
	virtual void addForce(Vector3 f);
	virtual void clearForce();
	virtual Vector3 getVelocity();
	virtual float getHeight();

	virtual PhysicActor* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 vel, float lt, BoundingBox* _bb);
};