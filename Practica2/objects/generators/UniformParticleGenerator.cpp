#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(Particle* model, Vector3 var, float pro, bool up)
	: ParticleGenerator(model, var, pro, up) {
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

	float random = (rand() % 101) / 100.0f;

	if (random < generation_probability) {
		// Variables aleatorias
		Vector3 vel = Vector3((*velX)(gen), (*velY)(gen), (*velZ)(gen));
		int lifeTime = rand() % 10 + 3;

		// Crear partícula
		if (!uniquePoint) {
			Vector3 pos = Vector3((*posX)(gen), (*posY)(gen), (*posZ)(gen));
			newParts.push_back(model_part->clone(pos, vel, lifeTime));
		}
		else newParts.push_back(model_part->clone(model_part->getPos(), vel, lifeTime));
	}

	return newParts;
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