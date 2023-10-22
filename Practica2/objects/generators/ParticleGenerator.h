#pragma once
#include <string>
#include <list>
#include <chrono>
#include <random>
#include "../physicObjects/Particle.h"
using namespace std;

class ParticleGenerator {
protected:
	// Caracteristicas del generador: nombre y si es estatico o no
	string name;
	bool uniquePoint;

	// Particula modelo y medias de vel, pos y varianza
	Particle* model_part = nullptr;
	Vector3 mean_pos, mean_vel, meanVar;

	// Cosas de probabilidades
	double generation_probability = 1.0;
	default_random_engine gen;

public:
	ParticleGenerator(Particle* p, Vector3 var, double prob, bool up);
	~ParticleGenerator();

	// Setters
	void setParticle(Particle* model, bool modify_pos_vel = true);
	inline void setOrigin(const Vector3& p) { mean_pos = p; }
	inline void setMeanVelocity(const Vector3& v) { mean_vel = v; }
	inline void setMeanDuration(double new_duration) { model_part->setLifeTime(new_duration); }

	// Getters
	inline string getName() { return name; }
	inline Vector3 getMeanVelocity() const { return mean_vel; }

	virtual list<Particle*> generateParticles() = 0;
};