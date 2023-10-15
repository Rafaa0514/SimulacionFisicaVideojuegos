#pragma once
#include "../../RenderUtils.hpp"

class Particle {
protected:
	// Parámetros de movimiento
	Vector3 vel;
	Vector3 acceleration;
	float damping;

	float lifeTime;
	double startTime;

	physx::PxTransform pose;
	RenderItem* renderItem;
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acce, float dp = 0.998f);
	virtual ~Particle();

	bool integrate(double t);

	virtual Particle* clone() const;
};