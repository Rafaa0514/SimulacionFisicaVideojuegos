#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
	
}

void ParticleSystem::update(double t) {
	for (auto it = myParticles.begin(); it != myParticles.end(); it++) {
		if (!(*it)->integrate(t)) {
			deadParticles.push_back(it);
		}
	}

	for (int i = 0; i < deadParticles.size(); i++) {
		myParticles.erase(deadParticles[i]);
	}
	deadParticles.clear();
}