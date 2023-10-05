#pragma once
#include "Particle.h"

enum ProyectileType { CANON, FIREBALL, BULLET, LASER };

// ----- VALORES -----
// Cañón:			Velocidad -> 50			Masa -> 2.9			Velocidad simulada -> 35
// Bola de fuego:	Velocidad -> 6			Masa -> 1 * 10^-7	Velocidad simulada -> 20
// Bala :			Velocidad -> 360		Masa -> 0.008		Velocidad simulada -> 72
// Laser :			Velocidad -> 300000000	Masa -> 0			Velocidad simulada -> 100

class Proyectile : public Particle {
protected:
	float mass, sim_mass;
	float real_vel, sim_vel;
public:
	Proyectile(Vector3 pos, Vector3 dir, ProyectileType id);
	virtual ~Proyectile();

	void assignParameters(ProyectileType id);

	float calculateSimMass();
	void calculateGravity();
};