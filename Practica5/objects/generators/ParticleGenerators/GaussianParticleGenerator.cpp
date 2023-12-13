#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string n, Particle* model, Vector3 var, float prob,
	ParticleForceRegistry* r, ForceGenerators& _fgs, bool up, Vector3 vp) :
	ParticleGenerator(n, model, var, prob, up, vp, r, _fgs) {
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
		Particle* newP = nullptr;
		// Crear partícula
		if (uniquePoint) newP = model_part->clone(model_part->getPos(), calculateVel());
		else newP = model_part->clone(calculatePos(), calculateVel());
		newParts.push_back(newP);
		pfr->addRegistry(fgs, newP);
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