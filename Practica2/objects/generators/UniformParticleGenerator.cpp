#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string n, Particle* model, Vector3 var, float pro, bool up)
	: ParticleGenerator(n, model, var, pro, up) {
	// Asignamos valores al intervalo de velocidad
	assignVel();
	// Si las partículas pueden crearse desde mas de un punto
	if (!uniquePoint) assignPos();
}

UniformParticleGenerator::~UniformParticleGenerator() {
	delete velX; delete velY; delete velZ;
	if (!uniquePoint) { delete posX; delete posY; delete posZ; }
}

list<Particle*> UniformParticleGenerator::generateParticles() {
	list<Particle*> newParts;

	if (rndProb(rg) < generation_probability) {
		// Crear partícula
		if (!uniquePoint) newParts.push_back(model_part->clone(calculatePos(), calculateVel(), GLOBAL_GRAVITY));
		else newParts.push_back(model_part->clone(model_part->getPos(), calculateVel(), GLOBAL_GRAVITY));
	}

	return newParts;
}

Vector3 UniformParticleGenerator::calculateVel() {
	return Vector3((*velX)(rg), (*velY)(rg), (*velZ)(rg));
}

Vector3 UniformParticleGenerator::calculatePos() {
	return Vector3((*posX)(rg), (*posY)(rg), (*posZ)(rg));
}

void UniformParticleGenerator::assignVel() {
	velX = new uniform_real_distribution<float>(mean_vel.x - meanVar.x, mean_vel.x + meanVar.x);
	velY = new uniform_real_distribution<float>(mean_vel.y - meanVar.y, mean_vel.y + meanVar.y);
	velZ = new uniform_real_distribution<float>(mean_vel.z - meanVar.z, mean_vel.z + meanVar.z);
}


void UniformParticleGenerator::assignPos() {
	posX = new uniform_real_distribution<float>(mean_pos.x - meanVar.x * 2, mean_pos.x + meanVar.x / 2.0f);
	posY = new uniform_real_distribution<float>(mean_pos.y - meanVar.y * 2, mean_pos.y + meanVar.y / 2.0f);
	posZ = new uniform_real_distribution<float>(mean_pos.z - meanVar.z * 2, mean_pos.z + meanVar.z / 2.0f);
}