#pragma once

#include <map>
#include <list>
#include "ForceGenerator.h"
using namespace std;
using ForceGenerators = std::list<ForceGenerator*>;

typedef pair<PhysicActor*, ForceGenerator*> FRPair;

class ActorForceRegistry : public multimap<PhysicActor*, ForceGenerator*> {
public:
	void updateForces() {
		for (auto it = begin(); it != end();) {
			if (it->second->updateForce(it->first)) it++;
			else it = erase(it);
		}
	}

	void addRegistry(ForceGenerator* fg, PhysicActor* p) {
		insert({ p, fg });
	}

	void addRegistry(ForceGenerator* fg, const std::list<PhysicActor*>& ps) {
		for (auto it = ps.begin(); it != ps.end(); it++) {
			insert({ *it, fg });
		}
	}

	void addRegistry(ForceGenerators fgs, const std::list<PhysicActor*>& ps) {
		for (auto it1 = fgs.begin(); it1 != fgs.end(); it1++) {
			for (auto it2 = ps.begin(); it2 != ps.end(); it2++) {
				insert({ *it2, *it1 });
			}
		}
	}

	void addRegistry(ForceGenerators fgs, PhysicActor* p) {
		for (auto it = fgs.begin(); it != fgs.end(); it++) {
			insert({ p, *it});
		}
	}

	void deleteActorRegistry(PhysicActor* p) {
		erase(p);
	}

	void deleteAllActorsRegistry(list<PhysicActor*> parts) {
		for (PhysicActor* p : parts) erase(p);
	}

	void deleteForceRegistry(ForceGenerator* fg) {
		for (auto it = begin(); it != end();) {
			if (it->second == fg) it = erase(it);
			else it++;
		}
	}
};