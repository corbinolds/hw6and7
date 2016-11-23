#include "Hero1.h"
#include "Hero.h"
#include "Point.h"
#include <math.h>
#include <fstream>			// we'll use ifstream
#include <string>			// for, well strings!
#include <vector>
#include <time.h>			// for time() to seed our RNG
#include <stdio.h>			// allow to print to terminal
#include <stdlib.h>			// access to rand() and exit()
#include <math.h>			// for cosf(), sinf(), etc.
#include <sstream>
#include <iostream>// and vectors (the storage container, not directional)
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
using namespace std;

Hero1::Hero1() {
	petCurveFileName = "hero1petCurve.csv";
	carX = 0;
	carY = 0;
	carZ = 0;
	carTheta = 0;
	carPhi = 0;
	petX = 0;
	petY = 0;
	petZ = 0;
	coneSize = 5.0f;
	bezHeight = 0;
	loadControlPointsForPet();
}

Hero1::Hero1(string petCurveFile) {
	petCurveFileName = petCurveFile;
	carX = 0;
	carY = 0;
	carZ = 0;
	carTheta = 0;
	carPhi = 0;
	coneSize = 5.0f;
	bezHeight = 0;
	petX = 0;
	petY = 0;
	petZ = 0;
	loadControlPointsForPet();
}

void Hero1::loadControlPointsForPet() {

	ifstream myfile(petCurveFileName.c_str());
	if (!myfile.is_open()) {
		std::cout << "The file couldn't be opened." << std::endl;
		exit(-1);
	}
	else {
		int number;
		myfile >> number;
		for (int i = 0; i <= number; i++)
		{
			vector<string> result;
			std::string line;
			std::getline(myfile, line);
			//std::cout<<line<<std::endl;
			std::stringstream lineStream(line);
			std::string cell;
			vector<float> test;
			while (std::getline(lineStream, cell, ','))
			{

				test.push_back(atof(cell.c_str()));
			}
			if (i != 0) {
				Point temp(test[0], test[1], test[2]);
				petControlPoints.push_back(temp);
			}
		}
	}

}
Point Hero1::evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3, float t) {
	// TODO #08: Compute a point along a Bezier curve
	float x = pow(1 - t, 3)*p0.getX() + 3 * pow(1 - t, 2)*t*p1.getX() + 3 * (1 - t)*pow(t, 2)*p2.getX() + pow(t, 3)*p3.getX();
	float y = pow(1 - t, 3)*p0.getY() + 3 * pow(1 - t, 2)*t*p1.getY() + 3 * (1 - t)*pow(t, 2)*p2.getY() + pow(t, 3)*p3.getY();
	float z = pow(1 - t, 3)*p0.getZ() + 3 * pow(1 - t, 2)*t*p1.getZ() + 3 * (1 - t)*pow(t, 2)*p2.getZ() + pow(t, 3)*p3.getZ();
	return Point(x, y, z);
}

void Hero1::renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution) {
	// TODO #07: Draw a Bezier curve
	glBegin(GL_LINES);
	for (int i = 0; i < resolution; i++) {
		float j = (i / (float)resolution);
		Point temp = evaluateBezierCurve(p0, p1, p2, p3, j);
		glVertex3f(temp.getX(), temp.getY(), temp.getZ());
	}
	glEnd();
}

void Hero1::drawWheel() {
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glColor3f(.5, .5, .5);
	glScalef(1.0, 1.0, 1.0);
	glutSolidTorus(.5, 1, 50, 50);
	glPopMatrix();
}
void Hero1::drawWheelsAndAxel() {
	glPushMatrix();
	glTranslatef(0, 0, 2);
	drawWheel();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -2);
	drawWheel();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 1, -2);
	glBegin(GL_POLYGON);
	glColor3f(.8, .5, .1);
	GLUquadricObj *obj = gluNewQuadric();
	gluCylinder(obj, .5, .5, 6, 30, 30);
	glEnd();
	glPopMatrix();
}
void Hero1::drawWheels() {
	glPushMatrix();
	glTranslatef(2, 0, 0);
	drawWheelsAndAxel();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0, 0);
	drawWheelsAndAxel();
	glPopMatrix();
}
void Hero1::drawBody() {
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glColor3f(.1, .3, .8);
	glScalef(5.0, 1, 2.0);
	glutSolidCube(1);
	glPopMatrix();
}
void Hero1::drawTop() {
	glPushMatrix();
	glTranslatef(1, 2, 0);
	glColor3f(.1, .1, .1);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(1, coneSize, 50, 50);
	glPopMatrix();
}
void Hero1::drawCar() {
	//glRotatef(90,0,1,0);
	glTranslatef(carX, carY, carZ);
	glPushMatrix();
	//glRotatef((carTheta * 180)/ M_PI,carX,carY,carZ);
	//glTranslatef(-carX, -carY, -carZ);
	glRotatef(-(((carTheta) * 180) / M_PI), 0, 1, 0);
	//glTranslatef(carX,carY,carZ);
	oldTheta = carTheta;
	drawBody();
	drawWheels();
	drawTop();
	glPopMatrix();

}

void Hero1::drawPoints() {
	// TODO #05: Draw our control points
	for (unsigned int i = 0; i < petControlPoints.size(); i++) {
		glPushMatrix();
		//std::cout << petControlPoints[i].getX() << " " << petControlPoints[i].getY() << " " << petControlPoints[i].getZ() << std::endl;
		glColor3f(0, 1, 0);
		glTranslatef(petControlPoints[i].getX(), petControlPoints[i].getY(), petControlPoints[i].getZ());
		glutSolidSphere(double(0.4), 20, 20);
		glPopMatrix();
	}
}

void Hero1::drawBox() {
	// TODO #06: Connect our control points
	glDisable(GL_LIGHTING);
	for (unsigned int i = 0; i < petControlPoints.size() - 1; i++) {
		glPushMatrix();
		glColor3f(1, 1, 0);
		glLineWidth(3.0f);	// make our grid lines a little thicker so they are easier to see
		glBegin(GL_LINES);
		glVertex3f(petControlPoints[i].getX(), petControlPoints[i].getY(), petControlPoints[i].getZ());
		glVertex3f(petControlPoints[i + 1].getX(), petControlPoints[i + 1].getY(), petControlPoints[i + 1].getZ());
		glEnd();
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
}

void Hero1::drawCurve() {
	// TODO #07: Draw the Bezier Curve!

	glDisable(GL_LIGHTING);
	for (unsigned int i = 0; i < petControlPoints.size() - 3; i += 3) {
		glPushMatrix();
		glColor3f(0, 0, 1);
		glLineWidth(3.0f);	// make our grid lines a little thicker so they are easier to see
		renderBezierCurve(petControlPoints[i], petControlPoints[i + 1], petControlPoints[i + 2], petControlPoints[i + 3], 5000);
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
}

void Hero1::drawPetBody() {
	glutSolidSphere(1, 50, 50);
}

void Hero1::drawPetLeftArm() {
	glutSolidSphere(1, 50, 50);
}
void Hero1::drawPetRightArm() {
	glutSolidSphere(1, 50, 50);
}
void Hero1::drawPetLeftEye() {
	glutSolidSphere(1, 50, 50);
}
void Hero1::drawPetRightEye() {
	glutSolidSphere(1, 50, 50);
}
void Hero1::drawPetMouth() {

	glColor3f(1, 0, 0);
	glTranslatef(.78, -.05, 0);
	glScalef(.25, .25, .25);
	glPushMatrix();
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
}
void Hero1::drawPetEyes() {
	glColor3f(0, 0, 0);
	glTranslatef(.8, .3, 0);
	glScalef(.25, .25, .25);
	glPushMatrix();
	glTranslatef(0, 0, -1);
	drawPetLeftEye();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 1);
	drawPetRightEye();
	glPopMatrix();
}
void Hero1::drawPetLegs() {
	//glPushMatrix();
	//glTranslatef()
	glColor3f(1, 0, 0);
	glTranslatef(0, -.6, 0);
	glScalef(.5, .5, .5);
	glPushMatrix();
	glTranslatef(0, 0, -1.5);
	drawPetLeftArm();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 1.5);
	drawPetRightArm();
	glPopMatrix();
}
void Hero1::drawPetArms() {
	glTranslatef(0, .6, 0);
	glScalef(.5, .5, .5);
	glPushMatrix();
	glTranslatef(0, 0, -1.5);
	drawPetLeftArm();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 1.5);
	drawPetRightArm();
	glPopMatrix();
}
void Hero1::drawPet() {
	glPushMatrix();
	//std::cout<< petX << " " << petY << " " << petZ << std::endl;
	glTranslatef(petX, petY, petZ);
	glColor3f(1, (float)105 / (float)255, (float)180 / (float)255);
	glScalef(petScale, petScale, petScale);
	glPushMatrix();
	drawPetBody();
	glPopMatrix();
	glPushMatrix();
	drawPetArms();
	glPopMatrix();
	glPushMatrix();
	drawPetLegs();
	glPopMatrix();
	glPushMatrix();
	drawPetEyes();
	glPopMatrix();
	glPushMatrix();
	drawPetMouth();
	glPopMatrix();
	glPopMatrix();
}

void Hero1::drawBezier() {
	glTranslatef(carX, carY, carZ);
	glTranslatef(0, 5, 0);
	glRotatef(-(((carTheta) * 180) / M_PI), 0, 1, 0);
	glPushMatrix();
	if (!hideControl) {
		//drawPoints();
	}
	glPopMatrix();
	glPushMatrix();
	if (!hideControl) {
		//drawBox();
	}
	glPopMatrix();
	glPushMatrix();
	if (!hideBezier) {
		//drawCurve();
	}
	glPopMatrix();
	glPushMatrix();
	drawPet();
	glPopMatrix();
	glPushMatrix(); {
		//glTraslatef(0, 0, 200);
		glScalef(.01, .01, .01);
		glColor3f(.8, .6, .4);
		glRotatef((270 / M_PI), 0, 1, 0);
		drawText("Minrak Steelhide");
	}; glPopMatrix();
}

void Hero1::drawText(char*name) {
	char *c;
	for (c = name; *c; c++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
}

void Hero1::draw() {
	glTranslatef(0, bezHeight, 0);
	glPushMatrix();


	drawCar();
	glPopMatrix();
	glPushMatrix();
	drawBezier();
	glPopMatrix();
	glPushMatrix();
	//drawPet();
	glPopMatrix();
}
void Hero1::animate() {
	if (petScale > 1.5) {
		petScale = 1.5;
		petIncrease = false;
	}
	if (petScale < 1) {
		petScale = 1;
		petIncrease = true;
	}
	if (petIncrease) {
		petScale += .01;
	}
	else {
		petScale -= .01;
	}
	if (moving) {
		if (!coneDecrease) {
			coneSize += .05;
		}
		else {
			coneSize -= .05;
		}
		if (coneSize > 10) {
			coneSize = 9.99;
			coneDecrease = true;
		}
		if (coneSize < 0) {
			coneSize = .001;
			coneDecrease = false;
		}
	}
	if (petControlPoints.size() - 1 <= controlIndex) {
		controlIndex = 0;
	}
	if (dis > 1) {
		dis = 0;
		controlIndex += 3;
	}
	Point temp = evaluateBezierCurve(petControlPoints[(int)controlIndex], petControlPoints[(int)controlIndex + 1], petControlPoints[(int)controlIndex + 2], petControlPoints[(int)controlIndex + 3], dis);
	petX = temp.getX();
	petY = temp.getY();
	petZ = temp.getZ();
	//distance = distance + .001;
	dis = dis + .005;
}

void Hero1::moveForward() {
	carX += cam.getMovementX();
	carZ += cam.getMovementZ();
	if (carX > 50 || carX < -50 || carZ > 50 || carZ < -50) {
		if (carX > 50) {
			carX = 50 - .001;
		}
		else if (carX < -50) {
			carX = -50 + .001;
		}
		if (carZ > 50) {
			carZ = 50 - .001;
		}
		else if (carZ < -50) {
			carZ = -50 + .001;
		}
	}
	else {
		//cameraX += cameraSpeed * dirX;
		//cameraZ += cameraSpeed * dirZ;
		cam.updateCameraIncreaseX(cam.getMovementX());
		cam.updateCameraIncreaseZ(cam.getMovementZ());
	}
}
void Hero1::moveBackward() {
	carX -= cam.getMovementX();
	carZ -= cam.getMovementZ();
	//std::cout << cam.getMovementX() << " " << cam.getMovementZ() << "dir x: " << cam.getDirX() << "dir y: " << cam.getDirY() << "dir z: " << cam.getDirZ() << " " << cam.getCameraSpeed() << std::endl;
	if (carX > 50 || carX < -50 || carZ > 50 || carZ < -50) {
		if (carX > 50) {
			carX = 50 - .001;
		}
		else if (carX < -50) {
			carX = -50 + .001;
		}
		if (carZ > 50) {
			carZ = 50 - .001;
		}
		else if (carZ < -50) {
			carZ = -50 + .001;
		}
	}
	else {
		//cameraX -= cameraSpeed * dirX;
		//cameraZ -= cameraSpeed * dirZ;
		cam.updateCameraDecreaseX(cam.getMovementX());
		cam.updateCameraDecreaseZ(cam.getMovementZ());
	}
}
void Hero1::turnLeft() {
	carTheta -= .05;
	updateFPCameraTheta(-.05);
}
void Hero1::turnRight() {
	carTheta += .05;
	updateFPCameraTheta(.05);
}
float Hero1::getBezH(){
	return bezHeight;
}
float Hero1::getCarTheta() {
	return carTheta;
}
float Hero1::getCarPhi() {
	return carPhi;
}
float Hero1::getCarX() {
	return carX;
}
float Hero1::getCarY() {
	return carY;
}
float Hero1::getCarZ() {
	return carZ;
}
void Hero1::setMoving(bool moving) {
	this->moving = moving;
}
Camera Hero1::getCamera() {
	//will have an if statment here.
	return cam;
}
Camera Hero1::getFirstPersonCamera() {
	//will have an if statment here.
	return firstPersonCamera;
}
/*
will have a get look at coordinates function
if arcball make it look at car position
else if first person look at dirx,diry,dirz
*/
void Hero1::updateCarY(float update){
	bezHeight = update;
}
void Hero1::updateCamera() {
	cam.recomputeOrientation(carTheta);
}
void Hero1::updateFirstPersonCamera() {
	firstPersonCamera.recomputeFCOrientation();
	//firstPersonCamera.recomputeFPOrientation(carTheta);
}
void Hero1::moveCamera(int totalLength) {
	cam.moveCamera(totalLength, carX, carY, carZ);
}

void Hero1::updateCameraPhi(float phi) {
	cam.updateCameraPhi(phi);
}

void Hero1::updateCameraTheta(float theta) {
	cam.updateCameraTheta(theta);
}
void Hero1::updateFPCameraPhi(float phi) {
	firstPersonCamera.updateFCCameraPhi(phi);
}

void Hero1::updateFPCameraTheta(float theta) {
	firstPersonCamera.updateFCCameraTheta(theta);
}