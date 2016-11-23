#ifndef PARTICLE
#define PARTICLE

#include "Point.h"
#include "Vector.h"
#include <math.h>
#include <stdlib.h>
#include <string>
using namespace std;

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Particle {
	Point position;
	double velocity;
	double radius;
	double angle;
	double particleAge;
public:
	Particle();
	Point getPosition();
	void setPosition(Point position);
	void initVelocity();
	void initAngle();
	double getParticleAge();
	void draw();
	void update();
	void drawSolidSphere();
};

#endif