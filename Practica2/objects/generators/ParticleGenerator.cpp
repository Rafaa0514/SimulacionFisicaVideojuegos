#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Particle* p, Vector3 var, double prob, bool up) : 
	meanVar(var), generation_probability(prob), uniquePoint(up) {
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
	//model_part->setPosition({ -1000.0f, -1000.0f, -1000.0f });
}