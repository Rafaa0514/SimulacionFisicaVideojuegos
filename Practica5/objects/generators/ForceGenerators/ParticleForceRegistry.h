#pragma once

#include <map>
#include <list>
#include "ForceGenerator.h"
using namespace std;
using ForceGenerators = std::list<ForceGenerator*>;

typedef pair<Particle*, ForceGenerator*> FRPair;

class ParticleForceRegistry : public multimap<Particle*, ForceGenerator*> {
public:
	void updateForces() {
		for (auto it = begin(); it != end();) {
			if (it->second->updateForce(it->first)) it++;
			else it = erase(it);
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) {
		insert({ p, fg });
	}

	void addRegistry(ForceGenerator* fg, const std::list<Particle*>& ps) {
		for (auto it = ps.begin(); it != ps.end(); it++) {
			insert({ *it, fg });
		}
	}

	void addRegistry(ForceGenerators fgs, const std::list<Particle*>& ps) {
		for (auto it1 = fgs.begin(); it1 != fgs.end(); it1++) {
			for (auto it2 = ps.begin(); it2 != ps.end(); it2++) {
				insert({ *it2, *it1 });
			}
		}
	}

	void addRegistry(ForceGenerators fgs, Particle* p) {
		for (auto it = fgs.begin(); it != fgs.end(); it++) {
			insert({ p, *it});
		}
	}

	void deleteParticleRegistry(Particle* p) {
		erase(p);
	}

	void deleteAllParticlesRegistry(list<Particle*> parts) {
		for (Particle* p : parts) erase(p);
	}

	void deleteForceRegistry(ForceGenerator* fg) {
		for (auto it = begin(); it != end();) {
			if (it->second == fg) it = erase(it);
			else it++;
		}
	}
};