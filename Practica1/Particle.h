#pragma once
#include "RenderUtils.hpp"

class Particle {
protected:
	// Parámetros de movimiento
	Vector3 vel;
	Vector3 acceleration;
	float damping;

	physx::PxTransform pose;
	RenderItem* renderItem;
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acce, float dp);
	virtual ~Particle();

	void integrate(double t);
};