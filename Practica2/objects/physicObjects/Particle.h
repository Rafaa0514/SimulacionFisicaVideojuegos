#pragma once
#include "../../RenderUtils.hpp"

// Parametros de la particula
// Posicion inicial, Velocidad inicial, tamaño inicial, color y tiempo de vida

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
	float radious;

public:
	Particle(Vector3 pos, Vector3 vel, float rad, Vector4 color, float lt, float dp = 0.998f);
	virtual ~Particle();

	bool integrate(double t);

	// Getters
	Vector3 getPos() { return pose.p; }
	Vector3 getVel() { return vel; }

	// Setters
	void setLifeTime(float const& lt) { lifeTime = lt; }
	void setPosition(Vector3 const& p) { pose.p = p; }

	virtual Particle* clone() const;
	virtual Particle* clone(Vector3 p, Vector3 v, float lt) const;
};