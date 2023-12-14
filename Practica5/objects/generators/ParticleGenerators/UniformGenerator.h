#pragma once
#include "ActorGenerator.h"

class UniformGenerator : public ActorGenerator {
protected:
	uniform_real_distribution<float>* velX;
	uniform_real_distribution<float>* velY;
	uniform_real_distribution<float>* velZ;

	uniform_real_distribution<float>* posX;
	uniform_real_distribution<float>* posY;
	uniform_real_distribution<float>* posZ;


public:
	UniformGenerator(string n, PxPhysics* g, PxScene* s, PhysicActor* model, Vector3 var, float pro,
		ActorForceRegistry* r, ForceGenerators& _fgs, bool up = true, Vector3 varP = Vector3(1));
	virtual ~UniformGenerator();

	// Metodos abstractos
	virtual list<PhysicActor*> generateActors();
	virtual Vector3 calculateVel();
	virtual Vector3 calculatePos();

	// Asignar valor a los intervalos
	void assignVel();
	void assignPos();
};