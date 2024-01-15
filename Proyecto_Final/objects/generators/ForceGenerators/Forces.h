#pragma once
#define _USE_MATH_DEFINES
#include "ForceGenerator.h"
#include <cmath>

// ----------GENERADOR DE GRAVEDAD----------
class GravityGenerator : public ForceGenerator {
protected:
	Vector3 grav;
public:
	GravityGenerator(Vector3 G, Vector3 bbC, Vector3 bbS, double d = -1, string n = "GRAV") : 
		ForceGenerator(n, d, bbC, bbS), grav(G) {}
	virtual bool updateForce(PhysicActor* p);
	void setGravity(const Vector3& g) { grav = g; }
};

//----------GENERADOR DE VIENTO----------
class WindGenerator : public ForceGenerator {
protected:
	float k1, k2;
	Vector3 wVel;
public:
	WindGenerator(float _k1, float _k2, Vector3 w, Vector3 bbC, Vector3 bbS, double d = -1, string n = "WIND") : 
		ForceGenerator(n, d, bbC, bbS), k1(_k1), k2(_k2), wVel(w) {}
	virtual bool updateForce(PhysicActor* p);

	void setDrag(float _k1, float _k2) { k1 = _k1; k2 = _k2; }
	void setWindVelocity(const Vector3& w) { wVel = w; }

	float getK1() { return k1; }
	float getK2() { return k2; }
};

//----------GENERADOR DE TORBELLINO----------
class TornadoGenerator : public WindGenerator {
protected:
	float k;
	Vector3 origin;


public:
	TornadoGenerator(float _k, float _k1, float _k2, Vector3 o, Vector3 w, Vector3 bbC, Vector3 bbS, double d = -1, string n = "TORN") :
		WindGenerator(_k1, _k2, w, bbC, bbS, d, n), k(_k), origin(o) {}
	virtual bool updateForce(PhysicActor* p);
};

//----------GENERADOR DE EXPLOSIONES----------
class ExplosionGenerator : public ForceGenerator {
protected:
	float rad, finalRad;
	float k;
	float tau;
	Vector3 center;
	

public:
	ExplosionGenerator(float r, float _k, float T, Vector3 c, double d = -1, string n = "EXPL") :
		ForceGenerator(n, d, Vector3(0), Vector3(0)), rad(r), finalRad(r), k(_k), tau(T), center(c) {}

	virtual bool updateForce(PhysicActor* p);
};

//----------GENERADORES DE MUELLES----------
class AnchoredSpringGenerator : public ForceGenerator {
protected:
	double k;
	double r_length;
	Vector3 point;

	RenderItem* renderItem;
	physx::PxTransform pose;

public:
	AnchoredSpringGenerator(double _k, double rl, Vector3 p, string name = "ANCH", double d = -1)
		: k(_k), r_length(rl), point(p), ForceGenerator(name, d, p, Vector3(1)), pose(p)
		,renderItem(new RenderItem(CreateShape(physx::PxBoxGeometry(Vector3(1))), &pose, colores[BLACK])) 
		{}
	virtual bool updateForce(PhysicActor* p);
	virtual ~AnchoredSpringGenerator() { 
		renderItem->release();
		renderItem = nullptr; 
	}

	// Getters
	double getK() { return k; }
	double getRL() { return r_length; }
	// Setters
	void setK(double _k) { k = _k; };
	void setRL(double _rl) { r_length = _rl; }
};

class SpringGenerator : public AnchoredSpringGenerator {
protected:
	PhysicActor* other;
	bool elastic;
public:
	SpringGenerator(double _k, double rl, PhysicActor* o, bool e = false) :
		AnchoredSpringGenerator(_k, rl, o->getPos()), other(o), elastic(e) {
		renderItem->release();
	}
	virtual bool updateForce(PhysicActor* p);
};

//----------GENERADOR DE FLOTACION----------
class BouyancyForceGenerator : public ForceGenerator {
protected:
	float height, volume, liquid_density;
	RenderItem* fluidLimit;
	physx::PxTransform pose;

public:
	BouyancyForceGenerator(float h, float d, Vector3 bbP, Vector3 bbS, double dur = -1) : 
		ForceGenerator("FLOT", dur, bbP, bbS), height(h), volume(0), liquid_density(d), pose(bbP),
		fluidLimit(new RenderItem(CreateShape(physx::PxBoxGeometry(Vector3(bbS.x, 0.1, bbS.z))), &pose, colores[YELLOW])) {}

	virtual bool updateForce(PhysicActor* p);
};