#pragma once
#include "../../RenderUtils.hpp"
#include "BoundingBox.h"
#include <iostream>

// Parametros de la particula
// Posicion inicial, Velocidad inicial, tamaño inicial, color y tiempo de vida

const Vector3 GLOBAL_GRAVITY = Vector3(0, -9.8f, 0);
const float RADIOUS = 2.0f;


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
	Vector4 col;

	Vector3 force;
	double inv_mass, mass;

	BoundingBox* bb;

public:
	Particle(Vector3 pos, Vector3 vel, float rad, double m, Vector4 color,
		double lt, BoundingBox* _bb, float dp = 0.998f);
	virtual ~Particle();

	virtual bool integrate(double t);

	// Getters
	Vector3 getPos()	{ return pose.p; }
	Vector3 getVel()	{ return vel; }
	Vector3 getAcc()	{ return acceleration; }
	Vector4 getColor()	{ return renderItem->color; }
	float getRadious()	{ return radious; }
	double getMass()	{ return mass; }
	double getInvMass()	{ return inv_mass; }

	// Setters
	void setVel(Vector3 v) { vel = v; }
	void setLifeTime(double const& lt)	{ lifeTime = lt; }
	void setPosition(Vector3 const& p)	{ pose.p = p; }
	void setMass(double m);

	virtual Particle* clone() const;
	virtual Particle* clone(Vector3 p, Vector3 v) const;

	void clearForce();
	void addForce(const Vector3& f);
	void removeRender() { renderItem->release(); renderItem = nullptr; }
};