#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
	Particle* model = new Particle(Vector3(0), Vector3(30), 4, Vector4(0, 255, 0, 1), 2);
	ParticleGenerator* pGen = new UniformParticleGenerator(model, Vector3(60), 0.01f);
	particleGenerators.push_back(pGen);
}

ParticleSystem::~ParticleSystem() {
	for (ParticleGenerator* pg : particleGenerators) delete pg;
	particleGenerators.clear();

	for (Particle* p : myParticles) delete p;
	myParticles.clear();
}

void ParticleSystem::update(double t) {

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