#include "ParticleSystem.h"
#include <iostream>
using namespace std;

ParticleSystem::ParticleSystem() {
	int spawnRate = 5;   //how many to draw per second
	int spawnFrameNumber = 60 / spawnRate;
	int counter = 60 / spawnRate;
}

void ParticleSystem::update() {
	if (counter% 12 == 0) {
		Particle newParticle;
		particles.push_back(newParticle);
	}
	for (unsigned int j = 0; j < particles.size(); j++) {
		if (particles.at(j).getParticleAge() >= 4.0) {
			particles.erase(particles.begin() + j);
		}
		else if (particles.at(j).getPosition().getY() <= 1 && particles.at(j).getParticleAge() > 0.1) {
			particles.erase(particles.begin() + j);
		}
		else {
			particles.at(j).update();
		}
	}
	counter++;
}

void ParticleSystem::draw() {
	for (unsigned int i = 0; i < particles.size(); i++) {
		particles.at(i).draw();
	}
}

