#include "Hero3.h"
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


//default constructor
Hero3::Hero3() {
	characterX = 0;
	characterY = 0;
	characterZ = 0;
	transportRotation = 0;
	wheelRotation = 0;
	fanRotate = 0;
	sphereRadius = 0.5f;
	lineThickness = 3.0f;
	trackPointVal = 0.0f;
	timerValue = 0.0f;
	trackTimerValue = 0.0f;
	DRAW_CAGE = false;
	DRAW_CURVE = true;
	WHICH_POINTS = true;
	WHICH_POINTS_TRACK = true;
	trackCurveFile = "Hero2trackCurve.csv";
	arcDistance = 0.0;
	tVal = 0.0f;
}

float Hero3::calcTVal() {
	float testVal;
	if (tVal >= 2 || arcLengthValues.size() != 500) {
		return 0;
	}
	else {
		for (float j = 0.0; j < 499.0; j += 1) {
			if (arcDistance > arcLengthValues.at(j) && arcDistance < arcLengthValues.at(j + 1)) {
				testVal = calcLerp(j / 250, (j + 1) / 250, arcDistance, arcLengthValues.at(j), arcLengthValues.at(j + 1));
				break;
			}
			else if (arcDistance == arcLengthValues.at(j)) {
				testVal = j / 500;
				break;
			}
		}
		return testVal;
	}
}
void Hero3::animate() {
	if (arcLengthValues.size() == 500 && arcDistance > arcLengthValues.at(499)) {
		arcDistance = 0;
	}	
	
	if (arcLengthValues.size() == 500) {
		arcDistance += arcLengthValues.at(499)/803;
		tVal = calcTVal();
	}

	if (tVal >= 2) {
		tVal = 0;
		arcDistance = 0;
	}
	else if (tVal < 1) {
		onTrack = evaluateBezierCurve(trackControlPoints.at(0), trackControlPoints.at(1), trackControlPoints.at(2), trackControlPoints.at(3), tVal);
	}
	else if(tVal <  2){
		onTrack = evaluateBezierCurve(trackControlPoints.at(3), trackControlPoints.at(4), trackControlPoints.at(5), trackControlPoints.at(6), tVal - 1);
	}
	fanRotate += 5;
}
void Hero3::draw() {
	glPushMatrix(); {
		drawTrackControlPoints();
		glTranslatef(onTrack.getX(), onTrack.getY(), onTrack.getZ());
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(characterX + sin(transportRotation*M_PI / 180), characterY + 2, characterZ + cos(transportRotation*M_PI / 180));
				glRotatef(transportRotation, 0, 1, 0);
				glScalef(.25, .25, .25);
			}; glPopMatrix();
			glPushMatrix(); {
				glTranslatef(0, 2, 0);
				glScalef(.01, .01, .01);
				glColor3f(.4, .4, .9);
				drawText("BurntShake");
			}; glPopMatrix();
			drawCharacter();
		}; glPopMatrix();
	}; glPopMatrix();
}

void Hero3::drawText(char*name) {
	char *c;
	for (c = name; *c; c++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
}

//draw cyclinder for character
//void drawCylinder()
void Hero3::drawCylinder() {
	float numSteps = 26.0f;
	float radius = 1.0f;
	float length = 10.0f;
	float hl = length * 0.5f;
	float count = 0.0f;
	float step = 3.14159 * 2 / numSteps;
	glPushMatrix(); {
		glColor3f(.7, .3, .3);
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i <= numSteps; i++) {
			float x = cos(count)*radius;
			float y = sin(count)*radius;
			glVertex3f(x, y, -hl);
			glVertex3f(x, y, hl);

			count += step;
		}
		glEnd();
	}; glPopMatrix();
}

//draw a fan on my cylinder
void Hero3::drawFan() {
	glPushMatrix(); {
		glColor3f(.3, .9, .1);
		glScalef(.075, .075, .075);
		glRotatef(fanRotate, 0, 0, 1);
		glBegin(GL_TRIANGLE_FAN);
		//first fan
		glVertex3f(0, 0, 0);
		glVertex3f(53, -53, 0);
		glVertex3f(20, -72, 0);
		//second fan
		glVertex3f(0, 0, 0);
		glVertex3f(-53, -53, 0);
		glVertex3f(-72, -20, 0);
		//third fan
		glVertex3f(0, 0, 0);
		glVertex3f(-53, 53, 0);
		glVertex3f(-20, 72, 0);
		//fourth fan
		glVertex3f(0, 0, 0);
		glVertex3f(53, 53, 0);
		glVertex3f(72, 20, 0);

		glEnd();

	};
	glPopMatrix();
}

//draw a copter for the back of my car
//void drawCopter()
void Hero3::drawCopter() {
	glPushMatrix(); {
		//glRotatef(90,1,0,0);
		glPushMatrix(); {
			glRotatef(90, 1, 0, 0);
			drawCylinder();
		}; glPopMatrix();
		glPushMatrix(); {
			glTranslatef(0, 6, 0);
			glRotatef(90, 1, 0, 0);
			drawFan();
		}; glPopMatrix();
	}; glPopMatrix();
}

//void draw the wheels of my car
//void drawWheels()
void Hero3::drawWheels() {
	glPushMatrix(); {
		//drawCylinder();

		glColor3f(.2, .1, .9);
		glPushMatrix(); {
			glTranslatef(0.5, 1, 0); //rotate the wheels around the axel
			glRotatef(wheelRotation, 0, 0, 1);
			glutSolidTorus(1, 2, 50, 20);
			//DON'T THINK THE WHEEL IS MOVING!?!? ME NEITHER
			//SO I PUT THIS RANDOM ASS TRIANGLE IN HERE TO PROVE IT UNCOMMENT IF YOU WANT PROOF
			// glBegin(GL_TRIANGLES); //begin random triangle //UNCOMMENT
			// glPushMatrix();{ //UNCOMMENT
			// glColor3f(.7,.3,.3); //UNCOMMENT
			// 	glVertex3f(5, 2*2, 0); //UNCOMMENT
			// 	glVertex3f(10, 2*2, 5); //UNCOMMENT
			// 	glVertex3f(0, 2*2, 5); //UNCOMMENT
			// glEnd(); //end random triangle //UNCOMMENT
			// };glPopMatrix(); //UNCOMMENT
			glTranslatef(-0.5, -1, 0);
		}; glPopMatrix();
		glPushMatrix(); {
			glTranslatef(10, 0, 0);
			glTranslatef(0.5, 1, 0);
			glRotatef(wheelRotation, 0, 0, 1);
			glutSolidTorus(1, 2, 50, 20);
			glTranslatef(-0.5, -1, 0);
		}; glPopMatrix();

		glPushMatrix(); {
			glTranslatef(10, 0, 10);
			glTranslatef(0.5, 1, 0);
			glRotatef(wheelRotation, 0, 0, 1);
			glutSolidTorus(1, 2, 50, 20);
			glTranslatef(-0.5, -1, 0);
		}; glPopMatrix();

		glPushMatrix(); {
			glTranslatef(0, 0, 10);
			glTranslatef(0.5, 1, 0);
			glRotatef(wheelRotation, 0, 0, 1);
			glutSolidTorus(1, 2, 50, 20);
			glTranslatef(-0.5, -1, 0);
		}; glPopMatrix();
		//glPushMatrix();

		//drawFan();
		//glPopMatrix();
	}; glPopMatrix();
}
//draw the body of the car
//void draw body
void Hero3::drawBody() {

	glPushMatrix(); {
		glBegin(GL_QUADS);
		//glColor3f(getRand(), getRand(), getRand());
		GLfloat height = 2;
		GLfloat x = 0;
		GLfloat z = 0;
		GLfloat length = 12;
		GLfloat width = 8;
		//bottom
		glVertex3f(x, 0, z);
		glVertex3f(x, 0, z + width);
		glVertex3f(x + length, 0, z + width);
		glVertex3f(x + length, 0, z);

		//top
		glVertex3f(x, height, z);
		glVertex3f(x, height, z + width);
		glVertex3f(x + length, height, z + width);
		glVertex3f(x + length, height, z);

		//side 1
		glVertex3f(x, 0, z);
		glVertex3f(x, 0, z + width);
		glVertex3f(x, height, z + width);
		glVertex3f(x, height, z);
		glVertex3f(x, 0, z);

		//side 2
		glVertex3f(x, 0, z + width);
		glVertex3f(x + length, 0, z + width);
		glVertex3f(x + length, height, z + width);
		glVertex3f(x, height, z + width);
		glVertex3f(x, 0, z + width);

		//side 3
		glVertex3f(x + length, 0, z + width);
		glVertex3f(x + length, 0, z);
		glVertex3f(x + length, height, z);
		glVertex3f(x + length, height, z + width);
		glVertex3f(x + length, 0, z + width);

		//side 4
		glVertex3f(x + length, 0, z);
		glVertex3f(x, 0, z);
		glVertex3f(x, height, z);
		glVertex3f(x + length, height, z);
		glVertex3f(x + length, 0, z);


		glEnd();
		// glBegin(GL_TRIANGLES);
		// glPushMatrix();{
		// glColor3f(.7,.3,.3);
		// 	glVertex3f(x/2, height*2, 0);
		// 	glVertex3f(x, height*2, z/2);
		// 	glVertex3f(0, height*2, z/2);
		// glEnd();
		// };glPopMatrix();
	}; glPopMatrix();


}
//draw my character that will move around the world
//void drawCharacter()
void Hero3::drawCharacter() {
	//glRotatef(transportRotation,0,1,0);
	glPushMatrix();

	glTranslatef(characterX, 0, characterZ);
	//glTranslatef(0,0,-4);
	glRotatef(transportRotation, 0, 1, 0);
	glScalef(.30f, .15f, .15f);


	glColor3f(.7, .3, .7);
	glPushMatrix(); {

		drawWheels();
		glPushMatrix(); {
			glTranslatef(0, 0, 5);
			drawCylinder();
			glTranslatef(10, 0, 0);
			drawCylinder();
		}; glPopMatrix();
	}; glPopMatrix();

	glPushMatrix(); {
		glColor3f(.3, .3, .7);
		glTranslatef(0, 0, 1);
		drawBody();
	}; glPopMatrix();
	glPushMatrix(); {
		glColor3f(.7, .3, .7);
		glTranslatef(10, 6, 4.5);
		drawCopter();
	}; glPopMatrix();
	//glTranslatef(0,0,4);
	glPopMatrix();
}
Point Hero3::evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3, float t) {
	// TODO #08: Compute a point along a Bezier curve
	float bezX = pow(1 - t, 3)*p0.getX() + 3 * pow(1 - t, 2)*t*p1.getX() + 3 * (1 - t)*pow(t, 2)*p2.getX() + pow(t, 3)*p3.getX();
	float bezY = pow(1 - t, 3)*p0.getY() + 3 * pow(1 - t, 2)*t*p1.getY() + 3 * (1 - t)*pow(t, 2)*p2.getY() + pow(t, 3)*p3.getY();
	float bezZ = pow(1 - t, 3)*p0.getZ() + 3 * pow(1 - t, 2)*t*p1.getZ() + 3 * (1 - t)*pow(t, 2)*p2.getZ() + pow(t, 3)*p3.getZ();

	return Point(bezX, bezY, bezZ);
}

float Hero3::calcLerp(float t1, float t2, float distance, float t1Dis, float t2Dis) {
	return t1 + (((distance - t1Dis) / (t2Dis - t1Dis)) * (t2 - t1));
}

void Hero3::renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution) {
	// TODO #07: Draw a Bezier curve
	//Point test1 = Point(0, 0, 0);
	//Point test2 = Point(1, 2, 3);
	glPushMatrix();
	glLineWidth(lineThickness);
	glColor3f(0, 0, 1);
	//glBegin(GL_LINE_STRIP);
	for (float i = 0; i < resolution; i += 1.0) {
		float time = float(i) / float(resolution);
		Point eval = evaluateBezierCurve(p0, p1, p2, p3, time);
		glVertex3f(eval.getX(), eval.getY(), eval.getZ());
		if (arcLengthValues.size() < 500) {
			bezierPoints.push_back(eval);
			if (i == 0) {
				arcLengthValues.push_back(0);
			}
			else {
				arcLengthValues.push_back(eval.getDistance(bezierPoints.at(i - 1)) + arcLengthValues.at(i - 1));
			}
		}
		//drawSolidSphere(eval.getX(),eval.getY(),eval.getZ());
	}
	//glEnd();
	glPopMatrix();
}
// drawSphere(float x,float y,float z)//
// draw a sphere at a location x, y, z
void Hero3::drawSphere(float inX, float inY, float inZ) {
	glPushMatrix(); {
		glColor3f(.2, .8, .2);
		glTranslatef(inX, inY, inZ);
		glutWireSphere(sphereRadius, 10, 10);
	}; glPopMatrix();
}


void Hero3::loadTrackControlPoints(vector<Point> controlPoints) {
	for (unsigned int i = 0; i < controlPoints.size(); i++) {
		trackControlPoints.push_back(controlPoints[i]);
	}
}

void Hero3::drawTrackControlPoints() {
	for (unsigned int i = 0; i < trackControlPoints.size(); i++) {
		if (DRAW_CAGE) {
			glPushMatrix(); {
				drawSphere(trackControlPoints.at(i).getX(), trackControlPoints.at(i).getY(), trackControlPoints.at(i).getZ());
			}; glPopMatrix();
			if (i < trackControlPoints.size() - 1) {
				glPushMatrix(); {
					glLineWidth(lineThickness);
					glColor3f(1, 1, 0);
					glBegin(GL_LINE_STRIP);
					glVertex3f(trackControlPoints.at(i).getX(), trackControlPoints.at(i).getY(), trackControlPoints.at(i).getZ());
					glVertex3f(trackControlPoints.at(i + 1).getX(), trackControlPoints.at(i + 1).getY(), trackControlPoints.at(i + 1).getZ());
					glEnd();
				}; glPopMatrix();
			}
		}
		if (i < trackControlPoints.size() - 3 && (i % 3 == 0) && DRAW_CURVE) {
			renderBezierCurve(trackControlPoints.at(i), trackControlPoints.at(i + 1), trackControlPoints.at(i + 2), trackControlPoints.at(i + 3), 500);
		}
	}
}

float Hero3::getCarX() {
	return onTrack.getX();
}
float Hero3::getCarY() {
	return onTrack.getY();
}
float Hero3::getCarZ() {
	return onTrack.getZ();
}
Camera Hero3::getCamera() {
	//will have an if statment here.
	return cam;
}
Camera Hero3::getFirstPersonCamera() {
	//will have an if statment here.
	return firstPersonCamera;
}
/*
will have a get look at coordinates function
if arcball make it look at car position
else if first person look at dirx,diry,dirz
*/
void Hero3::updateCamera() {
	//cam.recomputeOrientation(carTheta);
	cam.recomputeOrientation(0);
}
void Hero3::updateFirstPersonCamera() {
	firstPersonCamera.recomputeFCOrientation();
}
void Hero3::moveCamera(int totalLength) {
	cam.moveCamera(totalLength, getCarX(), getCarY(), getCarZ());
}

void Hero3::updateCameraPhi(float phi) {
	cam.updateCameraPhi(phi);
}

void Hero3::updateCameraTheta(float theta) {
	cam.updateCameraTheta(theta);
}
void Hero3::updateFPCameraPhi(float phi) {
	firstPersonCamera.updateFCCameraPhi(phi);
}

void Hero3::updateFPCameraTheta(float theta) {
	firstPersonCamera.updateFCCameraTheta(theta);
}