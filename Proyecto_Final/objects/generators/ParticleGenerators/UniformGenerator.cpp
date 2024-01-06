#include "UniformGenerator.h"

UniformGenerator::UniformGenerator(string n, PxPhysics* g, PxScene* s, PhysicActor* model, Vector3 var, float pro,
	ActorForceRegistry* r, ForceGenerators& _fgs, bool up, Vector3 varP)
	: ActorGenerator(n, g, s, model, var, pro, up, varP, r, _fgs) {
	// Asignamos valores al intervalo de velocidad
	assignVel();
	// Si las partículas pueden crearse desde mas de un punto
	if (!uniquePoint) assignPos();
}

UniformGenerator::~UniformGenerator() {
	delete velX; delete velY; delete velZ;
	if (!uniquePoint) { delete posX; delete posY; delete posZ; }
}

list<PhysicActor*> UniformGenerator::generateActors() {
	list<PhysicActor*> newActors;

	if (rndProb(rg) < generation_probability) {
		PhysicActor* newP = nullptr;
		// Crear partícula
		if (!uniquePoint) newP = model_act->clone(gPx, scene, calculatePos(), calculateVel(), model_act->getLT(), model_act->getBB());
		else newP = model_act->clone(gPx, scene, model_act->getPos(), calculateVel(), model_act->getLT(), model_act->getBB());

		newActors.push_back(newP);
		afr->addRegistry(fgs, newP);
	}

	return newActors;
}

Vector3 UniformGenerator::calculateVel() {
	return Vector3((*velX)(rg), (*velY)(rg), (*velZ)(rg));
}

Vector3 UniformGenerator::calculatePos() {
	return Vector3((*posX)(rg), (*posY)(rg), (*posZ)(rg));
}

void UniformGenerator::assignVel() {
	velX = new uniform_real_distribution<float>(mean_vel.x - meanVar.x, mean_vel.x + meanVar.x);
	velY = new uniform_real_distribution<float>(mean_vel.y - meanVar.y, mean_vel.y + meanVar.y);
	velZ = new uniform_real_distribution<float>(mean_vel.z - meanVar.z, mean_vel.z + meanVar.z);
}


void UniformGenerator::assignPos() {
	posX = new uniform_real_distribution<float>(mean_pos.x - varPos.x, mean_pos.x + varPos.x);
	posY = new uniform_real_distribution<float>(mean_pos.y - varPos.y, mean_pos.y + varPos.y);
	posZ = new uniform_real_distribution<float>(mean_pos.z - varPos.z, mean_pos.z + varPos.z);
}