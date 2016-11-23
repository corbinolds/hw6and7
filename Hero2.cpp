#include "Hero2.h"
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
Hero2::Hero2() {
	characterX = 0;
	characterY = 0;
	characterZ = 0;
	carYaw = 0;
	transportRotation = 0;
	wheelRotation = 0;
	fanRotate = 0;
	sphereRadius = 0.5f;
	lineThickness = 3.0f;
	trackPointVal = 0.0f;
	timerValue = 0.0f;
	trackTimerValue = 0.0f;
	//charName = { 'C','y','c','l','o','p','a','l','y','p','s','e','N','o','w', '\0' };
	char charName[32] = "CyclopalypseNow";
	DRAW_CAGE = false;
	DRAW_CURVE = true;
	WHICH_POINTS = true;
	WHICH_POINTS_TRACK = true;
	spiritCurveFile = "Hero2spiritCurve.csv";
	trackCurveFile = "Hero2trackCurve.csv";
	loadPetControlPoints();
	//loadTrackControlPoints();
}
//new pet file constructor
Hero2::Hero2(string petCurveFile) {
	characterX = 0;
	characterY = 0;
	characterZ = 0;
	transportRotation = 0;
	wheelRotation = 0;
	fanRotate = 0;
	//charName = "CyclopalypseNow";
	sphereRadius = 0.5f;
	lineThickness = 1.0f;
	trackPointVal = 0.0f;
	timerValue = 0.0f;
	trackTimerValue = 0.0f;
	DRAW_CAGE = true;
	DRAW_CURVE = true;
	WHICH_POINTS = true;
	WHICH_POINTS_TRACK = true;
	spiritCurveFile = petCurveFile;
	loadPetControlPoints();
}

void Hero2::animate() {
	if (timerValue >= 1.0) {
		timerValue = 0;
		WHICH_POINTS = !(WHICH_POINTS);
	}
	if (WHICH_POINTS) {
		onBez = evaluateBezierCurve(spiritControlPoints.at(0), spiritControlPoints.at(1), spiritControlPoints.at(2), spiritControlPoints.at(3), timerValue);

	}
	else {
		onBez = evaluateBezierCurve(spiritControlPoints.at(3), spiritControlPoints.at(4), spiritControlPoints.at(5), spiritControlPoints.at(6), timerValue);

	}
	if (trackTimerValue >= 1.0f) {
		trackTimerValue = 0;
		WHICH_POINTS_TRACK = !(WHICH_POINTS_TRACK);
	}
	if (WHICH_POINTS_TRACK) {
		onTrack = evaluateBezierCurve(trackControlPoints.at(0), trackControlPoints.at(1), trackControlPoints.at(2), trackControlPoints.at(3), trackTimerValue);
		onTrackNormal = evaluateTangent(trackControlPoints.at(0), trackControlPoints.at(1), trackControlPoints.at(2), trackControlPoints.at(3), trackTimerValue);
		previousTrack = evaluateBezierCurve(trackControlPoints.at(0), trackControlPoints.at(1), trackControlPoints.at(2), trackControlPoints.at(3), trackTimerValue-.0025f);
	}
	else {
		onTrack = evaluateBezierCurve(trackControlPoints.at(3), trackControlPoints.at(4), trackControlPoints.at(5), trackControlPoints.at(6), trackTimerValue);
		onTrackNormal = evaluateTangent(trackControlPoints.at(3), trackControlPoints.at(4), trackControlPoints.at(5), trackControlPoints.at(6), trackTimerValue);
		previousTrack = evaluateBezierCurve(trackControlPoints.at(3), trackControlPoints.at(4), trackControlPoints.at(5), trackControlPoints.at(6), trackTimerValue-.0025f);
	}
	carYaw = evaluateYaw(onTrack, previousTrack, onTrackNormal);
	carPitch = evaluatePitch(onTrack, previousTrack, onTrackNormal);
	//cout << carYaw	<< endl;
	timerValue += .01;
	trackTimerValue += .0025f;
	fanRotate += 5;
}
void Hero2::draw() {
	glPushMatrix(); {
		drawTrackControlPoints();
		glTranslatef(onTrack.getX(), onTrack.getY(), onTrack.getZ());
		//glRotatef(carYaw, 0, 1, 0);
		//glRotatef(carPitch, 1, 0, 0);
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(characterX + sin(transportRotation*M_PI / 180), characterY + 2, characterZ + cos(transportRotation*M_PI / 180));
				glRotatef(transportRotation, 0, 1, 0);
				glScalef(.25, .25, .25);
				//drawPetControlPoints();
				glPushMatrix(); {
					drawSpirit(onBez.getX(), onBez.getY(), onBez.getZ());

				}; glPopMatrix();
				glPushMatrix(); {
					glScalef(.05, .05, .05);
					glColor3f(.7, .2, .4);
					drawText("Cyclopalypse Now");

				}; glPopMatrix();
			}; glPopMatrix();
			drawCharacter();
		}; glPopMatrix();
	}; glPopMatrix();
}

void Hero2::drawText(char*name) {
	char *c;
	for (c = name; *c; c++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
}
//draw cyclinder for character
//void drawCylinder()
void Hero2::drawCylinder() {
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
void Hero2::drawFan() {
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
void Hero2::drawCopter() {
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
void Hero2::drawWheels() {
	glPushMatrix(); {
		//drawCylinder();

		glColor3f(255, 0.0, 0.0);
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
void Hero2::drawBody() {

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
void Hero2::drawCharacter() {
	//glRotatef(transportRotation,0,1,0);
	glPushMatrix();

	glTranslatef(characterX, 0, characterZ);
	//glTranslatef(0,0,-4);
	glRotatef(transportRotation, 0, 1, 0);
	glScalef(.15f, .15f, .15f);


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
//drawSphere for bez
void Hero2::drawBezSphere(float x, float y, float z){
	glPushMatrix();{
		glTranslatef(x,y,z);
		glScalef(.5,.5,.5);
		drawSolidSphere();
	};glPopMatrix();
}
// drawSolidSphere(float x,float y,float z)//
// draw a solid sphere at a location x, y, z
void Hero2::drawSolidSphere() {
	glPushMatrix(); {
		glColor4f(.2, .2, .8, .4);
		glutSolidSphere(sphereRadius, 10, 10);
	}; glPopMatrix();
}
//drawSipit(float x, float y, float z)
//draw the spirit for the bezier curve at x,y,z
void Hero2::drawSpirit(float inX, float inY, float inZ) {
	glPushMatrix(); {

		glTranslatef(inX, inY, inZ);
		// glRotatef(-fanRotate, 0,1,1);
		glPushMatrix(); {
			glScalef(2, 2, 2);
			drawSolidSphere();
		}; glPopMatrix();
		//One Sphere
		glPushMatrix(); {
			glRotatef(-fanRotate, 0, 1, 1);
			glTranslatef(1, 1, 0);
			drawSolidSphere();
		}; glPopMatrix();
		//Two Sphere
		glPushMatrix(); {
			glRotatef(-fanRotate, 1, 1, 0);
			glTranslatef(0, 1, 1);
			drawSolidSphere();
		}; glPopMatrix();
		//Three Sphere
		glPushMatrix(); {
			glRotatef(-fanRotate, 1, 0, 1);
			glTranslatef(1, -1, 0);
			drawSolidSphere();
		}; glPopMatrix();
		//Four Sphere
		glPushMatrix(); {
			glRotatef(-fanRotate, 1, -1, 0);
			glTranslatef(0, -1, 1);
			drawSolidSphere();
		}; glPopMatrix();
		// //Five Sphere
		// glPushMatrix();{
		// 	// glRotatef(-fanRotate, 0,1,-1);
		// 	glTranslatef(1,1,1);
		// 	drawSolidSphere();
		// };glPopMatrix();
		// //Six Sphere
		// glPushMatrix();{
		// 	// glRotatef(-fanRotate, -1,1,0);
		// 	glTranslatef(-1,-1,-1);
		// 	drawSolidSphere();
		// };glPopMatrix();
		// //Seven Sphere
		// glPushMatrix();{
		// 	// glRotatef(-fanRotate, 0,-1,1);
		// 	glTranslatef(1,-1,-1);
		// 	drawSolidSphere();
		// };glPopMatrix();
		// //Eight Sphere
		// glPushMatrix();{
		// 	//glRotatef(-fanRotate, 1,-1,0);
		// 	glTranslatef(-1,-1,1);
		// 	drawSolidSphere();
		// };glPopMatrix();
	}; glPopMatrix();
}

float Hero2::evaluatePitch(Point a, Point b, Point c){
	Point heading = Point(a.getX()-b.getX(), a.getY()-b.getY(), a.getZ()-b.getX());
	Point tangent = Point(a.getX()-c.getX(), a.getY()-c.getY(), a.getZ()-c.getZ());
	float vecAmag = heading.mag();
	float vecBmag = tangent.mag();

	float dotProduct = (heading.getX()*tangent.getX(),heading.getY()*tangent.getY(),heading.getZ()*tangent.getZ());
	return acos(dotProduct/(vecAmag	* vecBmag))*180/M_PI;
}
float Hero2::evaluateYaw(Point a, Point b, Point c){
	Point heading = Point(a.getX()-b.getX(), 0.0, a.getZ()-b.getX());
	Point tangent = Point(a.getX()-c.getX(), 0.0, a.getZ()-c.getZ());
	float vecAmag = heading.mag();
	float vecBmag = tangent.mag();

	float dotProduct = (heading.getX()*tangent.getX(),heading.getY()*tangent.getY(),heading.getZ()*tangent.getZ());
	return acos(dotProduct/(vecAmag	* vecBmag))*180/M_PI;
	//return 0;
}
Point Hero2::evaluateTangent(Point p0, Point p1, Point p2, Point p3, float t) {
float bezX = -3*pow(1 - t, 2)*p0.getX() + 3 * 2 * pow(1 - t, 1)*t*p1.getX() + 3 * 2 * (1 - t)*pow(t, 1)*p2.getX() + 3 * pow(t, 2)*p3.getX();
float bezY = -3*pow(1 - t, 2)*p0.getY() + 3 * 2 * pow(1 - t, 1)*t*p1.getY() + 3 * 2 * (1 - t)*pow(t, 1)*p2.getY() + 3 * pow(t, 2)*p3.getY();
float bezZ = -3*pow(1 - t, 2)*p0.getZ() + 3 * 2 * pow(1 - t, 1)*t*p1.getZ() + 3 * 2 * (1 - t)*pow(t, 1)*p2.getZ() + 3 * pow(t, 2)*p3.getZ();

return Point(bezX, bezY, bezZ);
}

Point Hero2::evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3, float t) {
	// TODO #08: Compute a point along a Bezier curve
	float bezX = pow(1 - t, 3)*p0.getX() + 3 * pow(1 - t, 2)*t*p1.getX() + 3 * (1 - t)*pow(t, 2)*p2.getX() + pow(t, 3)*p3.getX();
	float bezY = pow(1 - t, 3)*p0.getY() + 3 * pow(1 - t, 2)*t*p1.getY() + 3 * (1 - t)*pow(t, 2)*p2.getY() + pow(t, 3)*p3.getY();
	float bezZ = pow(1 - t, 3)*p0.getZ() + 3 * pow(1 - t, 2)*t*p1.getZ() + 3 * (1 - t)*pow(t, 2)*p2.getZ() + pow(t, 3)*p3.getZ();

	return Point(bezX, bezY, bezZ);
}
void Hero2::renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution) {
	// TODO #07: Draw a Bezier curve
	glPushMatrix();
	glLineWidth(lineThickness);
	glColor3f(0, 0, 1);
	//glBegin(GL_LINE_STRIP);
	for (float i = 0; i < resolution; i += .4) {
		float time = float(i) / float(resolution);
		Point eval = evaluateBezierCurve(p0, p1, p2, p3, time);
		glVertex3f(eval.getX(), eval.getY(), eval.getZ());
		//drawCylinder();
		drawBezSphere(eval.getX(),eval.getY(),eval.getZ());
	}
	//glEnd();
	glPopMatrix();
}
// drawSphere(float x,float y,float z)//
// draw a sphere at a location x, y, z
void Hero2::drawSphere(float inX, float inY, float inZ) {
	glPushMatrix(); {
		glColor3f(.2, .8, .2);
		glTranslatef(inX, inY, inZ);
		glutWireSphere(sphereRadius, 10, 10);
	}; glPopMatrix();
}
void Hero2::loadPetControlPoints() {
	// TODO #04: read in control points from file.  Make sure the file can be
	// opened and handle it appropriately.
	ifstream file(spiritCurveFile.c_str());
	if (!file.good()) {
		cout << "You're in for a bumpy ride" << endl;
		//exit(-1);
	}
	string numPoints;
	getline(file, numPoints);
	if (file.is_open()) {
		while (file.good()) {
			string new_Line;
			getline(file, new_Line);
			stringstream lineStream(new_Line);
			string point_Member;
			vector<double> input_Points;
			while (getline(lineStream, point_Member, ',')) {
				input_Points.push_back(atof(point_Member.c_str()));
			}
			//cout << "Error is here" << endl;
			if (file.good()) {
				Point new_Point(input_Points.at(0), input_Points.at(1), input_Points.at(2));
				spiritControlPoints.push_back(new_Point);
			}
		}
	}
	file.close();
	//return true;
}
//drawControlPoints()
//draw the bezier curve and it's control points
void Hero2::drawPetControlPoints() {
	// TODO #05: Draw our control points
	for (unsigned int i = 0; i < spiritControlPoints.size(); i++) {
		if (DRAW_CAGE) {
			glPushMatrix(); {
				drawSphere(spiritControlPoints.at(i).getX(), spiritControlPoints.at(i).getY(), spiritControlPoints.at(i).getZ());
			}; glPopMatrix();
			if (i < spiritControlPoints.size() - 1) {
				glPushMatrix(); {
					glLineWidth(lineThickness);
					glColor3f(1, 1, 0);
					glBegin(GL_LINE_STRIP);
					glVertex3f(spiritControlPoints.at(i).getX(), spiritControlPoints.at(i).getY(), spiritControlPoints.at(i).getZ());
					glVertex3f(spiritControlPoints.at(i + 1).getX(), spiritControlPoints.at(i + 1).getY(), spiritControlPoints.at(i + 1).getZ());
					glEnd();
				}; glPopMatrix();
			}
		}
		if (i < spiritControlPoints.size() - 3 && (i % 3 == 0) && DRAW_CURVE) {
			renderBezierCurve(spiritControlPoints.at(i), spiritControlPoints.at(i + 1), spiritControlPoints.at(i + 2), spiritControlPoints.at(i + 3), 200);
		}
	}
}

void Hero2::loadTrackControlPoints(vector<Point> controlPoints) {
	/*ifstream file(trackCurveFile.c_str());
	if (!file.good()) {
		cout << "You're in for a bumpy ride" << endl;
		//exit(-1);
	}
	string numPoints;
	getline(file, numPoints);
	if (file.is_open()) {
		while (file.good()) {
			string new_Line;
			getline(file, new_Line);
			stringstream lineStream(new_Line);
			string point_Member;
			vector<double> input_Points;
			while (getline(lineStream, point_Member, ',')) {
				input_Points.push_back(atof(point_Member.c_str()));
			}
			//cout << "Error is here" << endl;
			if (file.good()) {
				Point new_Point(input_Points.at(0), input_Points.at(1), input_Points.at(2));
				trackControlPoints.push_back(new_Point);
			}
		}
	}
	file.close();*/
	for (unsigned int i = 0; i < controlPoints.size(); i++) {
		trackControlPoints.push_back(controlPoints[i]);
	}
}

void Hero2::drawTrackControlPoints() {
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
			renderBezierCurve(trackControlPoints.at(i), trackControlPoints.at(i + 1), trackControlPoints.at(i + 2), trackControlPoints.at(i + 3), 200);
		}
	}
}

float Hero2::getCarX() {
	return onTrack.getX();
}
float Hero2::getCarY() {
	return onTrack.getY();
}
float Hero2::getCarZ() {
	return onTrack.getZ();
}
Camera Hero2::getCamera() {
	//will have an if statment here.
	return cam;
}
Camera Hero2::getFirstPersonCamera() {
	//will have an if statment here.
	return firstPersonCamera;
}
/*
will have a get look at coordinates function
if arcball make it look at car position
else if first person look at dirx,diry,dirz
*/
void Hero2::updateCamera() {
	//cam.recomputeOrientation(carTheta);
	cam.recomputeOrientation(0);
}
void Hero2::updateFirstPersonCamera() {
	firstPersonCamera.recomputeFCOrientation();
}
void Hero2::moveCamera(int totalLength) {
	cam.moveCamera(totalLength, getCarX(), getCarY(), getCarZ());
}

void Hero2::updateCameraPhi(float phi) {
	cam.updateCameraPhi(phi);
}

void Hero2::updateCameraTheta(float theta) {
	cam.updateCameraTheta(theta);
}
void Hero2::updateFPCameraPhi(float phi) {
	firstPersonCamera.updateFCCameraPhi(phi);
}

void Hero2::updateFPCameraTheta(float theta) {
	firstPersonCamera.updateFCCameraTheta(theta);
}