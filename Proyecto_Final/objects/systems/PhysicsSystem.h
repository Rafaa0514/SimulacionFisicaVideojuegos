#pragma once
#include "../generators/ParticleGenerators/UniformGenerator.h"
#include "../generators/ParticleGenerators/GaussianGenerator.h"
#include "../generators/ParticleGenerators/FireworkGenerator.h"
#include "../generators/ForceGenerators/Forces.h"
#include "../physicObjects/PhysicActor.h"
#include <list>
#include <vector>

using namespace physx;

class PhysicsSystem {
protected:
	PxPhysics* gPx;
	PxScene* scene;

	int particlesLimit = 100;
	PhysicActor* currentModel;
	std::list<PhysicActor*> myActors;
	std::list<ActorGenerator*> actorsGenerators;
	std::vector<list<PhysicActor*>::iterator> deadActors;

	std::list<Firework*> fireworks_pool;
	FireworkGenerator* fireworkGen;
	std::vector<list<Firework*>::iterator> deadFireworks;

	ActorForceRegistry* afr;
	ForceGenerators fgs;

	BoundingBox* bb;

	double timer;
	
	BoxParticle* floutingBox;


	RigidBody* floor;

public:
	PhysicsSystem(PxPhysics* g, PxScene* s);
	~PhysicsSystem();

	void update(double t);

	ActorGenerator* getActorGenerator(string name);

	void updateForcesTime(double t);

	// Distintos niveles
	void level1();
	void level2();
	void level3();

	void createActorGenerator(PhysicActor* model, Vector3 var_v, double prob, bool up = true, Vector3 var_p = Vector3(1));

	void clear();
};