#pragma once
#include "PhysicActor.h"
#include <iostream>

// Parametros de la particula
// Posicion inicial, Velocidad inicial, tamaño inicial, color y tiempo de vida

const Vector3 GLOBAL_GRAVITY = Vector3(0, -9.8f, 0);
const float RADIOUS = 2.0f;


enum Colors { RED, BLUE, YELLOW, GREEN, WHITE, BLACK };

const std::vector<Vector4> colores = {
	Vector4(1, 0, 0, 1),
	Vector4(0, 1, 1, 1),
	Vector4(1, 1, 0, 1),
	Vector4(0, 1, 0, 1),
	Vector4(1, 1, 1, 1),
	Vector4(0, 0, 0, 1)
};

class Particle : public PhysicActor {
protected:
	Vector3 vel, acc, force;
	float radious;
	Vector4 color;

public:
	Particle(Vector3 pos, Vector3 v, float rad, double m, Vector4 col, double lt, BoundingBox* _bb, float dp = 0.998f);


	// Getters
	virtual Vector3 getVelocity();
	virtual float getHeight();
	Vector3 getAcc() { return acc; }
	Vector4 getColor() { return color; }
	virtual float getRadious() { return radious; }

	// Setters
	void setVel(Vector3 v) { vel = v; }

	virtual bool integrate(double t);

	virtual bool collides(PhysicActor* other);

	virtual void addForce(Vector3 f);
	virtual void clearForce();
	
	virtual PhysicActor* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 vel, float lt, BoundingBox* _bb);
	virtual PhysicActor* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 vel, float lt,
		BoundingBox* _bb, double rnd);
	virtual Particle* clone();
};