#pragma once
#include "../generators/ParticleGenerators/UniformGenerator.h"
#include "../generators/ParticleGenerators/GaussianGenerator.h"
#include "../generators/ParticleGenerators/FireworkGenerator.h"
#include "../generators/ForceGenerators/Forces.h"
#include "../physicObjects/PhysicActor.h"
#include <list>
#include <vector>
#include <array>

using namespace physx;
const int NUM_LAYERS = 4;
const Vector3 CENTER_POSITION = Vector3(0, 25, 60);

class PhysicsSystem {
protected:

	int objectsLimit = 100;
	PhysicActor* currentModel;
	std::array<std::list<PhysicActor*>, NUM_LAYERS> myActors;
	std::list<ActorGenerator*> actorsGenerators;
	std::array<std::vector<list<PhysicActor*>::iterator>, NUM_LAYERS> deadActors;

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
	PxPhysics* gPx;
	PxScene* scene;

	PhysicsSystem(PxPhysics* g, PxScene* s);
	~PhysicsSystem();

	void update(double t);

	// Getters
	ActorGenerator* getActorGenerator(string name);
	std::list<PhysicActor*> getActorsFromLayer(Layer l) { return myActors[l]; }
	int getObjectsLimit() { return objectsLimit; }
	BoundingBox* getBB() { return bb; }

	// Setters
	void setGravity(Vector3 grav) { scene->setGravity(grav); }
	void setBB(Vector3 p, Vector3 s) { delete bb; bb = new BoundingBox(p, s); }
	void setObjectsLimit(int newLimit) { objectsLimit = newLimit; }

	// Añadir diferentes efectos fisicos
	void addActor(PhysicActor* newActor, Layer l = DEFAULT) { myActors[l].push_back(newActor); }
	void addForceGenerator(ForceGenerator* fg, Layer l = DEFAULT) { fgs.push_back(fg); afr->addRegistry(fg, myActors[l]); }
	void addForceAndActor(ForceGenerator* fg, PhysicActor* a, Layer l, bool addFG = true);
	void addFirework(Firework* f);

	void updateForcesTime(double t);

	void createActorGenerator(PhysicActor* model, Vector3 var_p, Vector3 var_v, double prob, Layer l, bool up = true, bool uv = true);

	void clear();
};