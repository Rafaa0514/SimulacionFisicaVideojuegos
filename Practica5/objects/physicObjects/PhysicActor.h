#pragma once
#include "../../RenderUtils.hpp"
#include "BoundingBox.h"

using namespace physx;

class PhysicActor {
protected:
	PxTransform pose;
	PxActor* act;
	double mass, inv_mass;
	float damping;

	float lifeTime;
	BoundingBox* bb;

	RenderItem* renderItem;

public:
	PhysicActor(Vector3 pos, double m, float lt = -1, BoundingBox* _bb = nullptr, float dp = 0.998);
	~PhysicActor();

	// Getters
	Vector3 getPos() { return pose.p; }
	double getMass() { return mass; }
	double getInvMass() { return inv_mass; }
	BoundingBox* getBB() { return bb; }
	float getLT() { return lifeTime; }

	// Setters
	void setPosition(Vector3 const& p) { pose.p = p; }
	void setLifeTime(double const& lt) { lifeTime = lt; }
	void setMass(double m);

	virtual bool integrate(double t);
	void releaseRender();

	// Metodos abstractos
	virtual void addForce(Vector3 f) = 0;
	virtual void clearForce() = 0;
	virtual Vector3 getVelocity() = 0;
	virtual float getHeight() = 0;

	virtual PhysicActor* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 vel, float lt, BoundingBox* _bb) = 0;
};