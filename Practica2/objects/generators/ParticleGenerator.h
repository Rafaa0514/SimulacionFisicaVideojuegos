#pragma once
#include <string>
#include <list>
#include <random>
#include "../physicObjects/Particle.h"

using namespace std;
using random_generator = std::mt19937;

class ParticleGenerator {
protected:
	// Caracteristicas del generador: nombre y si es estatico o no
	string name;
	bool uniquePoint;

	// Particula modelo y medias de vel, pos y varianza
	Particle* model_part = nullptr;
	Vector3 mean_pos, mean_vel, meanVar;
	Vector3 varPos;

	// Cosas de probabilidades
	double generation_probability = 1.0;

	// Cosas de aleatorios
	std::random_device rd;
	random_generator rg;
	uniform_real_distribution<> rndProb;

public:
	ParticleGenerator(string n, Particle* p, Vector3 var, double prob, bool up, Vector3 vp);
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
	virtual Vector3 calculateVel() = 0;
	virtual Vector3 calculatePos() = 0;
};