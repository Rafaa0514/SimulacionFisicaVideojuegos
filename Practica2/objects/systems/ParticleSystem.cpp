#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
	// ----- PARTÍCULAS -----
	Particle* model1 = new Particle(Vector3(0), Vector3(30), Vector3(0), 2, colores[GREEN], 2);
	ParticleGenerator* pGen1 = new UniformParticleGenerator("El uniforme", model1, Vector3(60), 0.1f);
	particleGenerators.push_back(pGen1);

	 Particle* model2 = new Particle(Vector3(0), Vector3(30), Vector3(0), 6, colores[RED], 2);
	 ParticleGenerator* pGen2 = new GaussianParticleGenerator("El Gaussiano", model2, Vector3(60), 0.1f, false);
	 particleGenerators.push_back(pGen2);


	// ----- FUEGOS ARTIFICIALES -----
	/*Firework* fModel1 = new Firework(Vector3(-50, 0, -100), Vector3(0,100,0), Vector3(0), colores[RED], 4.0f, 3, 8);
	fireworks_pool.push_back(fModel1);

	Firework* fModel2 = new Firework(Vector3(-200, 0, -300), Vector3(0, 100, 0), Vector3(0,-10,0), colores[GREEN], 5.0f, 3, 7);
	fireworks_pool.push_back(fModel2);

	fireworkGen = new FireworkGenerator("FIREWORKS BABY", Vector3(30, 100, 30));*/
}

ParticleSystem::~ParticleSystem() {
	for (ParticleGenerator* pg : particleGenerators) delete pg;
	particleGenerators.clear();

	for (Particle* p : myParticles) delete p;
	myParticles.clear();
}

void ParticleSystem::update(double t) {
	// ----- PARTÍCULAS -----
	for (ParticleGenerator* p : particleGenerators) {
		list<Particle*> prtcls = p->generateParticles();
		if (!prtcls.empty()) myParticles.splice(myParticles.end(), prtcls);
	}

	for (auto it = myParticles.begin(); it != myParticles.end(); it++) {
		if (!(*it)->integrate(t)) {
			deadParticles.push_back(it);
		}
	}

	for (int i = 0; i < deadParticles.size(); i++) {
		delete* deadParticles[i];
		myParticles.erase(deadParticles[i]);
	}
	deadParticles.clear();

	// ----- FUEGOS ARTIFICIALES -----
	/*for (auto it = fireworks_pool.begin(); it != fireworks_pool.end(); it++) {
		if (!(*it)->integrate(t)) {
			deadFireworks.push_back(it);
		}
	}

	for (int i = 0; i < deadFireworks.size(); i++) {
		if ((*deadFireworks[i])->getGeneration() > 1) {
			list<Firework*> fs = fireworkGen->generateFireworks(*deadFireworks[i]);
			if (!fireworks_pool.empty()) fireworks_pool.splice(fireworks_pool.end(), fs);
		}
		delete* deadFireworks[i];
		fireworks_pool.erase(deadFireworks[i]);
	}
	deadFireworks.clear();*/
}

ParticleGenerator* ParticleSystem::getParticleGenerator(string name) {
	auto it = particleGenerators.begin();
	bool found = false;

	while (!found && it != particleGenerators.end()) {
		found = (*it)->getName() == name;
		if (!found) it++;
	}

	return (*it);
}