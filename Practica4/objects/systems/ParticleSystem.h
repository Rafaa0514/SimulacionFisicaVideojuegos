#pragma once
#include "../generators/ParticleGenerators/UniformParticleGenerator.h"
#include "../generators/ParticleGenerators/GaussianParticleGenerator.h"
#include "../generators/ParticleGenerators/FireworkGenerator.h"
#include "../generators/ForceGenerators/Forces.h"
#include <list>
#include <vector>

enum ForceToShow { NONE, GRAV, WIND, TORN, EXPL, ANCH, SPRI, ELAS, BOUY, ALL };

class ParticleSystem {
protected:
	int particlesLimit = 100;
	Particle* currentModel;
	std::list<Particle*> myParticles;
	std::list<ParticleGenerator*> particleGenerators;
	std::vector<list<Particle*>::iterator> deadParticles;

	std::list<Firework*> fireworks_pool;
	FireworkGenerator* fireworkGen;
	std::vector<list<Firework*>::iterator> deadFireworks;

	ParticleForceRegistry* pfr;
	ForceGenerators fgs;

	BoundingBox* bb;

	ForceToShow fts;
	double timer;
	bool exploded;
	bool existGrav, existWind;
	BoxParticle* floutingBox;

public:
	ParticleSystem();
	~ParticleSystem();

	void update(double t);

	ParticleGenerator* getParticleGenerator(string name);

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

	void createParticleGenerator(Particle* model, Vector3 var_v, double prob, bool up = true, Vector3 var_p = Vector3(1));

	void clear();
};