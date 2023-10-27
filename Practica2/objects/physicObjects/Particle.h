#pragma once
#include "../../RenderUtils.hpp"

// Parametros de la particula
// Posicion inicial, Velocidad inicial, tamaño inicial, color y tiempo de vida

const Vector3 GLOBAL_GRAVITY = Vector3(0, -10, 0);
const float VERTICAL_LIMIT = -200.0f;


enum Colors { RED, BLUE, YELLOW, GREEN, WHITE, BLACK };

const std::vector<Vector4> colores = {
	Vector4(255, 0, 0, 1),
	Vector4(0, 255, 255, 1),
	Vector4(255, 255, 0, 1),
	Vector4(0, 255, 0, 1),
	Vector4(255, 255, 255, 1),
	Vector4(0, 0, 0, 1)
};

class Particle {
protected:
	// Parámetros de movimiento
	Vector3 vel;
	Vector3 acceleration;
	float damping;

	double lifeTime;

	physx::PxTransform pose;
	RenderItem* renderItem;
	float radious;

public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acc, float rad, Vector4 color, double lt, float dp = 0.998f);
	virtual ~Particle();

	virtual bool integrate(double t);

	// Getters
	Vector3 getPos() { return pose.p; }
	Vector3 getVel() { return vel; }
	Vector3 getAcc() { return acceleration; }
	Vector4 getColor() { return renderItem->color; }
	float getRadious() { return radious; }

	// Setters
	void setLifeTime(double const& lt) { lifeTime = lt; }
	void setPosition(Vector3 const& p) { pose.p = p; }

	virtual Particle* clone() const;
	virtual Particle* clone(Vector3 p, Vector3 v, Vector3 acc) const;
};