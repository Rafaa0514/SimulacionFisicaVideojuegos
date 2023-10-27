#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string n, Particle* p, Vector3 var, double prob, bool up, Vector3 vp) :
	name(n), meanVar(var), generation_probability(prob), uniquePoint(up), rg(rd()), rndProb(0.0, 1.0), varPos(vp) {
	setParticle(p);
	srand((unsigned)time);
}

ParticleGenerator::~ParticleGenerator() {
	delete model_part;
}

void ParticleGenerator::setParticle(Particle* model, bool modify_pos_vel) {
	delete model_part;
	model_part = model->clone();
	if (modify_pos_vel) {
		mean_pos = model_part->getPos();
		mean_vel = model_part->getVel();
	}
}