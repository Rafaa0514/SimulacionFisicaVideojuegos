#include "FireworkGenerator.h"

FireworkGenerator::FireworkGenerator(string n, Vector3 var, bool up) :
	name(n), mean_var(var), uniquePoint(up), rg(rd()) {
	
}

FireworkGenerator::~FireworkGenerator() {
	delete velX; delete velY; delete velZ;
}

list<Firework*> FireworkGenerator::generateFireworks(Firework* f) {
	assignVel(f->getVel());
	list<Firework*> newFs;
	for (int i = 0; i < f->getNumberSons(); i++) {
		float newLT = calculateLT(f->getLifetime(), f->getGeneration());
		int sons = calculateSons(f->getNumberSons());
		if (uniquePoint) 
			newFs.push_back(f->clone(f->getPos(), calculateVel(), newLT, sons));
		else 
			newFs.push_back(f->clone(calculatePos(f->getPos(), f->getRadious()), calculateVel(), newLT, sons));
	}
	return newFs;
}

void FireworkGenerator::assignVel(Vector3 const& v) {
	if (velX != nullptr) {
		delete velX; delete velY; delete velZ;
	}

	velX = new uniform_real_distribution<float>(v.x - mean_var.x, v.x + mean_var.x);
	velY = new uniform_real_distribution<float>(v.y - mean_var.y, v.y + mean_var.y);
	velZ = new uniform_real_distribution<float>(v.z - mean_var.z, v.z + mean_var.z);
}

Vector3 FireworkGenerator::calculateVel() {
	return Vector3((*velX)(rg), (*velY)(rg), (*velZ)(rg));
}

Vector3 FireworkGenerator::calculatePos(Vector3 const& p, float r) {
	uniform_int_distribution<>* posX = new uniform_int_distribution<>(p.x - r, p.x + r);
	uniform_int_distribution<>* posY = new uniform_int_distribution<>(p.y - r, p.y + r);
	uniform_int_distribution<>* posZ = new uniform_int_distribution<>(p.z - r, p.z + r);

	Vector3 newP = Vector3((*posX)(rg), (*posY)(rg), (*posZ)(rg));
	delete posX; delete posY; delete posZ;
	return newP;
}


float FireworkGenerator::calculateLT(float ltF, int g) {
	return (float) (ltF / g) * (g + 1);
}

int FireworkGenerator::calculateSons(int fSons) {
	if (sons != nullptr) delete sons;
	sons = new uniform_int_distribution<>(fSons - (fSons / 2), fSons + (fSons / 2));
	return (*sons)(rg);
}