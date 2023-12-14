#pragma once
#include "../generators/ParticleGenerators/UniformGenerator.h"
#include "../generators/ParticleGenerators/GaussianGenerator.h"
#include "../generators/ParticleGenerators/FireworkGenerator.h"
#include "../generators/ForceGenerators/Forces.h"
#include "../physicObjects/PhysicActor.h"
#include <list>
#include <vector>

using namespace physx;

enum ForceToShow { NONE, GRAV, WIND, TORN, EXPL, ANCH, SPRI, ELAS, BOUY, ALL };

class ParticleSystem {
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

	ForceToShow fts;
	double timer;
	bool exploded;
	bool existGrav, existWind;
	BoxParticle* floutingBox;

public:
	ParticleSystem(PxPhysics* g, PxScene* s);
	~ParticleSystem();

	void update(double t);

	ActorGenerator* getActorGenerator(string name);

	void generateFireworkSystem();

	void updateForcesTime(double t);

	// Para los distintos casos a probar
	void showGravityForce();
	void showWindForce();
	void showTornadoForce();
	void showExplosionForce();
	void createExplosionForce();

	// Para los casos de muelles
	void showSpringForce();
	void showAnchoredSpringForce();
	void showElastic();
	void showBouyancyForce();

	void addGravity();
	void addWind();
	void changeK(bool increase);
	void changeDimensions(bool increase);
	void changeMass(bool increase);

	void createActorGenerator(PhysicActor* model, Vector3 var_v, double prob, bool up = true, Vector3 var_p = Vector3(1));

	void clear();
};