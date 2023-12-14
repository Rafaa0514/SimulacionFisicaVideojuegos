#pragma once
#include "../../physicObjects/Firework.h"
#include "../ForceGenerators/ActorForceRegistry.h"
#include <random>

using namespace std;
using random_generator = std::mt19937;

class FireworkGenerator {
protected:
	string name;
	bool uniquePoint;
	Vector3 mean_var;

	uniform_real_distribution<float>* velX = nullptr;
	uniform_real_distribution<float>* velY = nullptr;
	uniform_real_distribution<float>* velZ = nullptr;

	uniform_int_distribution<>* sons = nullptr;

	// Cosas de aleatorios
	std::random_device rd;
	random_generator rg;

	// Generadores de fuerzas
	ActorForceRegistry* pfr;
	ForceGenerators& fgs;

public:
	FireworkGenerator(string n, ActorForceRegistry* r, ForceGenerators& _fgs, Vector3 var, bool up = true);
	~FireworkGenerator();

	virtual list<Firework*> generateFireworks(Firework* father);
	virtual void assignVel(Vector3 const& v);
	virtual Vector3 calculateVel();
	virtual Vector3 calculatePos(Vector3 const& p, float r);
	virtual double calculateLT(double ltF, int g);
	virtual int calculateSons(int fSons);
};