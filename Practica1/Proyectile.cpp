#include "Proyectile.h"

Proyectile::Proyectile(Vector3 pos, Vector3 dir, ProyectileType id) : Particle(pos, dir, Vector3(0,0,0), 0.99f){
	assignParameters(id);
}

Proyectile::~Proyectile() {

}

float Proyectile::calculateSimMass() {
	if (mass <= 0.1f) return 0.1f;
	return mass * powf(real_vel / sim_vel, 2);
}

void Proyectile::calculateGravity() {
	acceleration.y = -9.8f * powf(sim_vel / real_vel, 2);
}

void Proyectile::assignParameters(ProyectileType id) {
	switch (id) {
		case CANON:
		{
			real_vel = 450;  sim_vel = 45; vel.normalize(); vel *= sim_vel;
			mass = 2.9f; sim_mass = calculateSimMass();
			renderItem->color = Vector4(0,0,0,1);
			
		}break;

		case FIREBALL:
		{
			real_vel = 60;  sim_vel = 6; vel.normalize(); vel *= sim_vel;
			mass = 0.01f; sim_mass = calculateSimMass();
			renderItem->color = Vector4(230, 0, 0, 1);
		}break;

		case BULLET:
		{
			real_vel = 360;  sim_vel = 36; vel.normalize(); vel *= sim_vel;
			mass = 1.0f; sim_mass = calculateSimMass();
			renderItem->color = Vector4(255, 211, 0, 1);
		}break;

		case LASER:
		{
			real_vel = 300000000;  sim_vel = 100; vel.normalize(); vel *= sim_vel;
			mass = 0.0f; sim_mass = calculateSimMass();
			renderItem->color = Vector4(18, 126, 255, 1);
		}break;
	}

	calculateGravity();
}