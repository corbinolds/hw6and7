#include "Particle.h"
#include <iostream>

using namespace std;

Particle::Particle() {
	position = Point(0.0, 1.0, 0.0);
	velocity = 0.0;
	radius = 1.0;
	angle = 0.0;
	particleAge = 0.0;
	initAngle();
	initVelocity();
}


Point Particle::getPosition() {
	return this->position;
}

void Particle::setPosition(Point position) {
	this->position = position;
}

void Particle::initAngle() {
	int maxAngle = 30;
	int minAngle = 0;
	int randAngle = rand() % (maxAngle - minAngle + 1) + minAngle;
}

void Particle::initVelocity() {
	int maxVelocity = 35;
	int minVelocity = 15;
	int randVelocity = rand() % (maxVelocity - minVelocity + 1) + minVelocity;
	this->velocity = double(randVelocity);
}

double Particle::getParticleAge() {
	return this->particleAge;
}

void Particle::update() {
	double newPosition = (velocity*particleAge) + (0.5 * (-9.81 / 60) *pow(particleAge, 2));
	double newVelocity = velocity + ((-9.81 / 60) * particleAge);
	position.setY(newPosition);
	particleAge += (1.0 / 60.0);
	this->velocity = newVelocity;
}

void Particle::draw() {
	glPushMatrix(); {
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(position.getX(), position.getY(), position.getZ());
		drawSolidSphere();
	}; glPopMatrix();
}

void Particle::drawSolidSphere() {
	glPushMatrix(); {
		glutSolidSphere(radius, 10, 10);
	}; glPopMatrix();
}