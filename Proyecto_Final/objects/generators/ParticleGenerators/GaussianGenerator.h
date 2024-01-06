#pragma once
#include "ActorGenerator.h"

class GaussianGenerator : public ActorGenerator {
protected:
	normal_distribution<float>* velX;
	normal_distribution<float>* velY;
	normal_distribution<float>* velZ;

	normal_distribution<float>* posX;
	normal_distribution<float>* posY;
	normal_distribution<float>* posZ;

public:
	GaussianGenerator(string n, PxPhysics* g, PxScene* s, PhysicActor* model, Vector3 vel, float prob,
		ActorForceRegistry* r, ForceGenerators& _fgs, bool up = true, Vector3 var_pos = Vector3(1));
	virtual ~GaussianGenerator();

	// Metodos abstractos
	virtual list<PhysicActor*> generateActors();
	virtual Vector3 calculateVel();
	virtual Vector3 calculatePos();

	// Asignar valor a los intervalos
	void assignVel();
	void assignPos();
};