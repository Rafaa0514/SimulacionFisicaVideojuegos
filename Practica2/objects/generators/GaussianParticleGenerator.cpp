#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string n, Particle* model, Vector3 var, float prob, bool up, Vector3 vp) : 
	ParticleGenerator(n, model, var, prob, up, vp) {
	// Distribuciones de velocidad
	assignVel();
	// Distribuciones de posición
	if (!up) assignPos();
}

GaussianParticleGenerator::~GaussianParticleGenerator() {
	delete velX; delete velY; delete velZ;
	if (!uniquePoint) { delete posX; delete posY; delete posZ; }
}

list<Particle*> GaussianParticleGenerator::generateParticles() {
	list<Particle*> newParts;
	
	if (rndProb(rg) < generation_probability) {
		// Crear partícula
		if (uniquePoint) newParts.push_back(model_part->clone(model_part->getPos(), calculateVel(), GLOBAL_GRAVITY));
		else newParts.push_back(model_part->clone(calculatePos(), calculateVel(), GLOBAL_GRAVITY));
	}

	return newParts;
}

Vector3 GaussianParticleGenerator::calculateVel() {
	return Vector3((*velX)(rg), (*velY)(rg), (*velZ)(rg));
}

Vector3 GaussianParticleGenerator::calculatePos() {
	return Vector3((*posX)(rg), (*posY)(rg), (*posZ)(rg));
}

void GaussianParticleGenerator::assignVel() {
	velX = new normal_distribution<float>(mean_vel.x, meanVar.x);
	velY = new normal_distribution<float>(mean_vel.y, meanVar.y);
	velZ = new normal_distribution<float>(mean_vel.z, meanVar.z);
}


void GaussianParticleGenerator::assignPos() {
	posX = new normal_distribution<float>(mean_pos.x, varPos.x);
	posY = new normal_distribution<float>(mean_pos.y, varPos.y);
	posZ = new normal_distribution<float>(mean_pos.z, varPos.z);
}