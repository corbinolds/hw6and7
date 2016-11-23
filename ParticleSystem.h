#ifndef PARTICLESYSTEM
#define PARTICLESYSTEM

#include "Particle.h"
#include <vector>
using namespace std;

class ParticleSystem {
	vector<Particle> particles;
	int spawnRate;
	int spawnFrameNumber;
	int counter;
public:
	ParticleSystem();
	void draw();
	void update();
};

#endif