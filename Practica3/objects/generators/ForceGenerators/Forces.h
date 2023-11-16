#pragma once
#include "ForceGenerator.h"

// ----------GENERADOR DE GRAVEDAD----------
class GravityGenerator : public ForceGenerator {
protected:
	Vector3 grav;
public:
	GravityGenerator(Vector3 G, Vector3 bbC, Vector3 bbS, double d = -1, string n = "GRAV") : 
		ForceGenerator(n, d, bbC, bbS), grav(G) {}
	virtual bool updateForce(Particle* p);
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
	virtual bool updateForce(Particle* p);

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
	virtual bool updateForce(Particle* p);
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

	virtual bool updateForce(Particle* p);
};