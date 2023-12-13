#pragma once
#include "../../RenderUtils.hpp"

using namespace physx;

class PhysicActor {
protected:
	PxTransform pose;
	Vector3 vel, acc, force;

	double mass, inv_mass;
	float damping;

public:
	PhysicActor(Vector3 pos, Vector3 v, double m, float dp = 0.998) : 
		pose(pos), vel(v), acc(Vector3(0)), force(Vector3(0)), damping(dp) {
		setMass(m);
	}

	// Getters
	Vector3 getPos() { return pose.p; }
	Vector3 getVel() { return vel; }
	Vector3 getAcc() { return acc; }
	double getMass() { return mass; }
	double getInvMass() { return inv_mass; }

	// Setters
	void setVel(Vector3 v) { vel = v; }
	void setPosition(Vector3 const& p) { pose.p = p; }
	void setMass(double m) {
		if (m <= 0.0) inv_mass = 0;
		else inv_mass = 1.0f / m;

		mass = m;
	}

	
};