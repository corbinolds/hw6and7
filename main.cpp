/*
*  CSCI 441, Computer Graphics, Fall 2015
*
*  Project: lab02
*  File: main.cpp
*
*	Author: Dr. Jeffrey Paone - Fall 2015
*
*  Description:
*      Contains the base code for a basic flight simulator.
*
*/
//
#ifndef OUR_MAIN
#define OUR_MAIN
#include <GL/glew.h>

#ifdef __APPLE__			// if compiling on Mac OS
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

//#include <GL/glui.h>			// include our GLUI header
////

// C Libraries we need
#include <time.h>			// for time() to seed our RNG
#include <stdio.h>			// allow to print to terminal
#include <stdlib.h>			// access to rand() and exit()
#include <math.h>			// for cosf(), sinf(), etc.
#include <sstream>
// C++ Libraries we'll use
#include <fstream>			// we'll use ifstream
#include <string>			// for, well strings!
#include <vector>
#include <iostream>// and vectors (the storage container, not directional)
using namespace std;
//

/// Headers We've Written
#include "Point.h"
#include "Vector.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "Hero.h"
#include "Hero1.h"
#include "Hero2.h"
#include "Hero3.h"
#include "Shape.h"
// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth = 640;
static size_t windowHeight = 480;
static size_t miniWindowWidth = 640 / 3;
static size_t miniWindowHeight = 480 / 3;
static float aspectRatio;

GLint leftMouseButton; 		   	    // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse

float cameraX, cameraY, cameraZ;            // camera position in cartesian coordinates
float cameraRadius, cameraTheta, cameraPhi;
float carRadius, carTheta, carPhi; // camera DIRECTION in spherical coordinates
float dirX, dirY, dirZ;
float carX, carY, carZ;
float petX, petY, petZ;
float totalLength;
float coneSize;
float oldTheta;
float dis, petScale;
bool firstPersonCam = false;
bool isFreeCam = false;
float globalResolution = 200;
unsigned int controlIndex;
int viewPortIndex;
int currentIndex;
bool moving, coneDecrease, ctrlIsPressed, hideControl, hideBezier, petIncrease;                   // camera DIRECTION in cartesian coordinates
unsigned int milliseconds = 1000 / 60;
GLuint environmentDL;                       // display list for the 'city'
GLUquadric* qobj;
vector<Point> trackControlPoints;
vector<Point> bezierSurfaceControlPoints;
Camera freeCam(0,0,0,1,M_PI);
float updateFps = 0;
vector<Shape> objects;
//
//Hero currentHero;
Hero1* kylesHero = new Hero1();
Hero2* petersHero = new Hero2();
Hero3* corbinsHero = new Hero3();

ParticleSystem particleSystem;

//Hero1 *abc = new Hero1;
//Hero1 theReal;
//abc = & theReal;
//Hero1* pointerTest = &theReal;

Hero* currentHero;
vector<Hero*> heroes;
//Arbitrary Heros


//Camera cam;
//
// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
	return rand() / (float)RAND_MAX;
}
//

void CalculateFrameRate() {
	float static fps = 0.0f;
	float static lastTime = 0.0f; 
	float currentTime = GetTickCount() * 0.001f;
	++fps;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		if (fps >= 20) {
			updateFps = fps - 20;
		}
		else {
			updateFps = fps;
		}
		fps = 0;
	}
}

Point evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3, float t) {
	// TODO #08: Compute a point along a Bezier curve
	float x = pow(1 - t, 3)*p0.getX() + 3 * pow(1 - t, 2)*t*p1.getX() + 3 * (1 - t)*pow(t, 2)*p2.getX() + pow(t, 3)*p3.getX();
	float y = pow(1 - t, 3)*p0.getY() + 3 * pow(1 - t, 2)*t*p1.getY() + 3 * (1 - t)*pow(t, 2)*p2.getY() + pow(t, 3)*p3.getY();
	float z = pow(1 - t, 3)*p0.getZ() + 3 * pow(1 - t, 2)*t*p1.getZ() + 3 * (1 - t)*pow(t, 2)*p2.getZ() + pow(t, 3)*p3.getZ();
	return Point(x, y, z);
}
//
Point evaluateBezierPatch(Point p0, Point p1, Point p2, Point p3, 
						Point p4, Point p5, Point p6, Point p7, 
						Point p8, Point p9, Point p10, Point p11, 
						Point p12, Point p13, Point p14, Point p15, 
						float u, float v){
	Point patchPoint = evaluateBezierCurve(
		evaluateBezierCurve(p0,p1,p2,p3,u),
		evaluateBezierCurve(p4,p5,p6,p7,u),
		evaluateBezierCurve(p8,p9,p10,p11,u),
		evaluateBezierCurve(p12,p13,p14,p15,u),
		v);
	return patchPoint;
}
//

void myTimer(int value) {
	//animate all heroes
	kylesHero->animate();
	petersHero->animate();
	corbinsHero->animate();

	CalculateFrameRate();
	/*for (unsigned int i = 0; i < heroes.size(); i++) {
		heroes[i]->animate();
	}*/
	//basically anywhere there is a kyleshero it will switch to the "current hero and we move the camera based on that"

	//move peter and corbins hero here since they will be moving on the roller coaster
	//peterHero.move();
	//corbinHero.move();
	//
	Point temp = evaluateBezierPatch(bezierSurfaceControlPoints.at(0),bezierSurfaceControlPoints.at(1), bezierSurfaceControlPoints.at(2), bezierSurfaceControlPoints.at(3),
			bezierSurfaceControlPoints.at(4),bezierSurfaceControlPoints.at(5),bezierSurfaceControlPoints.at(6), bezierSurfaceControlPoints.at(7),
			bezierSurfaceControlPoints.at(8),bezierSurfaceControlPoints.at(9),bezierSurfaceControlPoints.at(10), bezierSurfaceControlPoints.at(11),
			bezierSurfaceControlPoints.at(12),bezierSurfaceControlPoints.at(13),bezierSurfaceControlPoints.at(14), bezierSurfaceControlPoints.at(15), (kylesHero->getCarX()+50)*2/globalResolution, (kylesHero->getCarZ()+50)*2/globalResolution);
	kylesHero->updateCarY(temp.getY());
	corbinsHero->moveCamera(totalLength);
	corbinsHero->updateCamera();
	petersHero->moveCamera(totalLength);
	petersHero->updateCamera();
	glutPostRedisplay();
	glutTimerFunc(milliseconds, myTimer, 0);
	particleSystem.update();
}
// drawGrid() //////////////////////////////////////////////////////////////////
//
//  Function to draw a grid in the XZ-Plane using OpenGL 2D Primitives (GL_LINES)
//
////////////////////////////////////////////////////////////////////////////////
void drawGrid() {
	/*
	*	We will get to why we need to do this when we talk about lighting,
	*	but for now whenever we want to draw something with an OpenGL
	*	Primitive - like a line, quad, point - we need to disable lighting
	*	and then reenable it for use with the GLUT 3D Primitives.
	*/
	glDisable(GL_LIGHTING);

	/** TODO #3: DRAW A GRID IN THE XZ-PLANE USING GL_LINES **/
	glBegin(GL_QUADS);
	glColor3f(.3, .4, .9);
	for (int i = 0; i <= globalResolution; i++) {
		// glVertex3f(i, 0, -50);
		// glVertex3f(i, 0, 50);
		// glVertex3f(-50, 0, i);
		// glVertex3f(50, 0, i);
		for(int j = 0; j < globalResolution; j++){//
			float timeI = float(i) / float(globalResolution);
			float timeJ = float(j) / float(globalResolution);
			Point vert = evaluateBezierPatch(bezierSurfaceControlPoints.at(0),bezierSurfaceControlPoints.at(1), bezierSurfaceControlPoints.at(2), bezierSurfaceControlPoints.at(3),
			bezierSurfaceControlPoints.at(4),bezierSurfaceControlPoints.at(5),bezierSurfaceControlPoints.at(6), bezierSurfaceControlPoints.at(7),
			bezierSurfaceControlPoints.at(8),bezierSurfaceControlPoints.at(9),bezierSurfaceControlPoints.at(10), bezierSurfaceControlPoints.at(11),
			bezierSurfaceControlPoints.at(12),bezierSurfaceControlPoints.at(13),bezierSurfaceControlPoints.at(14), bezierSurfaceControlPoints.at(15), timeJ, timeI);
			glVertex3f(vert.getX(), vert.getY(), vert.getZ());
			glVertex3f(vert.getX(), vert.getY(), vert.getZ()+1);
			glVertex3f(vert.getX()+1, vert.getY(), vert.getZ()+1);
			glVertex3f(vert.getX()+1, vert.getY(), vert.getZ());
		}

	}
	glEnd();
	/*
	*	As noted above, we are done drawing with OpenGL Primitives, so we
	*	must turn lighting back on.
	*/
	glEnable(GL_LIGHTING);
}
///
// drawCity() //////////////////////////////////////////////////////////////////
//
//  Function to draw a random city using GLUT 3D Primitives
//
////////////////////////////////////////////////////////////////////////////////
void drawCity() {
	// TODO #4: Randomly place buildings of varying heights with random colors
	for (int i = -50; i <= 50; i++) {
		for (int j = -50; j <= 50; j++) {
			float temp = getRand();
			float height = 1;
			do {
				height = getRand() * 10;
			} while (height < 1);
			if (temp < .1 && i % 2 == 0 && j % 2 == 0) {
				glPushMatrix();
				glTranslatef(i, height / 2, j);
				glColor3f(getRand(), getRand(), getRand());
				glScalef(1.0, height, 1.0);
				glutSolidCube(1);

				glPopMatrix();
			}
		}
	}
}

void drawObjects() {
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i].getName() == "sphere") {
			glPushMatrix();
			glTranslatef(objects[i].getLocation().getX(), objects[i].getLocation().getY(), objects[i].getLocation().getZ());
			//gl
			glColor3f(getRand(), getRand(), getRand());
			glScalef(objects[i].getSize(), objects[i].getSize(), objects[i].getSize());
			//glRotatef(0, objects[i].getOrientation.getX(), objects[i].getOrientation.getY(), objects[i].getOrientation.getZ());
			glutSolidSphere(1,50,50);

			glPopMatrix();
		}
		else if (objects[i].getName() == "cube") {
			glPushMatrix();
			glTranslatef(objects[i].getLocation().getX(), objects[i].getLocation().getY(), objects[i].getLocation().getZ());
			//gl
			glColor3f(getRand(), getRand(), getRand());
			//glRotatef(0, objects[i].getOrientation.getX(), objects[i].getOrientation.getY(), objects[i].getOrientation.getZ());
			glScalef(objects[i].getSize(), objects[i].getSize(), objects[i].getSize());
			glutSolidCube(1);

			glPopMatrix();
		}
	}
}
//

// renderBezierCurve() //////////////////////////////////////////////////////////
//
// Responsible for drawing a Bezier Curve as defined by four control points.
//  Breaks the curve into n segments as specified by the resolution.
//
////////////////////////////////////////////////////////////////////////////////
void renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution) {
	// TODO #07: Draw a Bezier curve
	glBegin(GL_LINES);
	for (int i = 0; i < resolution; i++) {
		float j = (i / (float)resolution);
		Point temp = evaluateBezierCurve(p0, p1, p2, p3, j);
		glVertex3f(temp.getX(), temp.getY(), temp.getZ());
	}
	glEnd();
}
// generateEnvironmentDL() /////////////////////////////////////////////////////
//
//  This function creates a display list with the code to draw a simple
//      environment for the user to navigate through.
//
//  And yes, it uses a global variable for the display list.
//  I know, I know! Kids: don't try this at home. There's something to be said
//      for object-oriented programming after all.
//
////////////////////////////////////////////////////////////////////////////////
void generateEnvironmentDL() {
	// TODO #1 Create a Display List & Call our Drawing Functions
	GLsizei n = 1;
	environmentDL = glGenLists(n);

	glNewList(environmentDL, GL_COMPILE);
	glPushMatrix(); {
		//drawCity();
		drawGrid();
		drawObjects();
	}; glPopMatrix();
	glEndList();
}


// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
void resizeWindow(int w, int h) {
	aspectRatio = w / (float)h;

	windowWidth = w;
	windowHeight = h;

	miniWindowWidth = w / 3;
	miniWindowHeight = h / 3;


	//update the viewport to fill the window
	if (viewPortIndex == 0) {
		glViewport(0, 0, w, h);
	}
	if (viewPortIndex == 1) {
		glViewport(0,0, miniWindowWidth, miniWindowHeight);
	}

	//update the projection matrix with the new window properties
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, 0.1, 100000);
}



// mouseCallback() /////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY) {
	// update the left mouse button states, if applicable
	if (button == GLUT_LEFT_BUTTON) {
		leftMouseButton = state;
		mouseX = thisX;
		mouseY = thisY;
	}
	if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		ctrlIsPressed = true;
		//mouseY = thisY;
		//glutGetModifiers() == GLUT_ACTIVE_CTRL
	}
	else {
		ctrlIsPressed = false;
	}

}

// mouseMotion() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse movement. We update cameraPhi, cameraTheta, and/or
//      cameraRadius based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y) {
	if (leftMouseButton == GLUT_DOWN) {
		if (ctrlIsPressed) {
			float dy = y - mouseY;
			float scale = .1;
			dy = dy * scale;
			totalLength += dy;
			if (totalLength < 10) {
				totalLength = 10.001;
			}
			if (totalLength > 200) {
				totalLength = 200 - .001;
			}

		}
		else {
			float dy = y - mouseY;
			float dx = x - mouseX;
			float scaleX = .0005;
			float scaleY = .01;
			dy = dy * scaleY;
			dx = dx * scaleX;

			//change to current hero 
			if (!isFreeCam) {
				switch (currentIndex) {
				case 0:
					if (!firstPersonCam) {
						kylesHero->updateCameraTheta(dx);
						kylesHero->updateCameraPhi(dy);
					}
					if (firstPersonCam) {
						kylesHero->updateFPCameraTheta(dx);
						kylesHero->updateFPCameraPhi(dy);
					}
					break;
				case 1:
					if (!firstPersonCam) {
						petersHero->updateCameraTheta(dx);
						petersHero->updateCameraPhi(dy);
					}
					if (firstPersonCam) {
						petersHero->updateFPCameraTheta(dx);
						petersHero->updateFPCameraPhi(dy);
					}
				case 2:
					if (!firstPersonCam) {
						corbinsHero->updateCameraTheta(dx);
						corbinsHero->updateCameraPhi(dy);
					}
					if (firstPersonCam) {
						corbinsHero->updateFPCameraTheta(dx);
						corbinsHero->updateFPCameraPhi(dy);
					}
					break;

				}
			}
			else {
				freeCam.updateFCCameraTheta(dx);
				freeCam.updateFCCameraPhi(dy);
			}
		}
		if (!isFreeCam) {
			switch (currentIndex) {
			case 0:
				if (!firstPersonCam) {
					kylesHero->moveCamera(totalLength);
					mouseY = y;
					kylesHero->updateCamera();
				}
				if (firstPersonCam) {
					//kylesHero->moveCamera(totalLength);
					mouseY = y;
					kylesHero->updateFirstPersonCamera();
				}
				break;
			case 1:
				if (!firstPersonCam) {
					petersHero->moveCamera(totalLength);
					mouseY = y;
					petersHero->updateCamera();
				}
				if (firstPersonCam) {
					//kylesHero->moveCamera(totalLength);
					mouseY = y;
					petersHero->updateFirstPersonCamera();
				}
				break;
			case 2:
				if (!firstPersonCam) {
					corbinsHero->moveCamera(totalLength);
					mouseY = y;
					corbinsHero->updateCamera();
				}
				if (firstPersonCam) {
					//kylesHero->moveCamera(totalLength);
					mouseY = y;
					corbinsHero->updateFirstPersonCamera();
				}
				break;
			}
		}
		else {
			//freeCam.moveCamera(totalLength, freeCam.getCameraX(), freeCam.getCameraY(), freeCam.getCameraZ());
			mouseY = y;
			freeCam.recomputeFCOrientation();
		}
	}
	//


	glutPostRedisplay();	    // redraw our scene from our new camera POV

}

// myMenu() /////////////////////////////////////////////////////////////////////
//
//  Handles our Menu events
//
////////////////////////////////////////////////////////////////////////////////
void myMenu(int value) {
	// TODO #02: handle our menu options
	if (value == 0) {
		exit(0);
	}
	/*
	if (value == 1) {
		currentHero = kylesHero;
		currentIndex = 0;
	}
	if (value == 2) {
		currentHero = corbinsHero;
		currentIndex = 2;
	}
	if (value == 3) {
		currentHero = petersHero;
		currentIndex = 1;
	}
	if (value == 4) {
		isFreeCam = false;
		firstPersonCam = false;
	}*/
	if (value == 1) {
		isFreeCam = true;
		firstPersonCam = false;

	}
	/*
	if (value == 6) {
		isFreeCam = false;
		firstPersonCam = true;
	}*/
}

void processArcBall(int value) {
	isFreeCam = false;
	if (value == 1) {
		currentHero = kylesHero;
		currentIndex = 0;
		firstPersonCam = false;
	}
	if (value == 2) {
		currentHero = corbinsHero;
		currentIndex = 2;
		firstPersonCam = false;
	}
	if (value == 3) {
		currentHero = petersHero;
		currentIndex = 1;
		firstPersonCam = false;
	}
}

void processFirstPerson(int value) {
	isFreeCam = false;
	if (value == 1) {
		currentHero = kylesHero;
		currentIndex = 0;
		firstPersonCam = true;
	}
	if (value == 2) {
		currentHero = corbinsHero;
		currentIndex = 2;
		firstPersonCam = true;
	}
	if (value == 3) {
		currentHero = petersHero;
		currentIndex = 1;
		firstPersonCam = true;
	}
}

void createMenus() {
	// TODO #01: Create a Simple Menu
	/*
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Quit", 0);
	glutAddMenuEntry("Show Hero 1 The Wanderer", 1);
	glutAddMenuEntry("Show Hero 2 Arc Parameterization", 2);
	glutAddMenuEntry("Show Hero 3 Same Step Size", 3);
	glutAddMenuEntry("Arc Ball Cam", 4);
	glutAddMenuEntry("Free Cam", 5);
	glutAddMenuEntry("First Person Cam", 6);
	glutAttachMenu(2);
	shrinkMenu = glutCreateMenu(processShrinkMenu);
	glutAddMenuEntry("Shrink", SHRINK);
	glutAddMenuEntry("NORMAL", NORMAL);
	*/

	int arcBall = glutCreateMenu(processArcBall);
	glutAddMenuEntry("The Wanderer", 1);
	glutAddMenuEntry("Arc Parameterization", 2);
	glutAddMenuEntry("Same Step Size", 3);

	int firstPerson = glutCreateMenu(processFirstPerson);
	glutAddMenuEntry("The Wanderer", 1);
	glutAddMenuEntry("Arc Parameterization", 2);
	glutAddMenuEntry("Same Step Size", 3);

	glutCreateMenu(myMenu);
	glutAddSubMenu("Arc Ball Cam", arcBall);
	glutAddSubMenu("First Person Cam", firstPerson);
	glutAddMenuEntry("Free Cam", 1);
	glutAddMenuEntry("Quit", 0);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
//
// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene() {
	glEnable(GL_DEPTH_TEST);

	//******************************************************************
	// this is some code to enable a default light for the scene;
	// feel free to play around with this, but we won't talk about
	// lighting in OpenGL for another couple of weeks yet.
	float lightCol[4] = { 1, 1, 1, 1 };
	float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
	float lPosition[4] = { 10, 10, 10, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// tell OpenGL not to use the material system; just use whatever we
	// pass with glColor*()
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//******************************************************************

	glShadeModel(GL_FLAT);

	srand(time(NULL));	// seed our random number generator
	generateEnvironmentDL();
}

//

// renderScene() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a scene to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(.5, .5, .5, 1);

	for (int i = 0; i <= 1; i++) {
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		viewPortIndex = i;
		if (viewPortIndex == 0) {
			glDisable(GL_SCISSOR_TEST);
			glViewport(0, 0, windowWidth, windowHeight);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0, aspectRatio, 0.1, 100000);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		if (viewPortIndex == 1 && firstPersonCam) {
			glViewport(0,0, miniWindowWidth, miniWindowHeight);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0, aspectRatio, 0.1, 100000);
			
			glScissor(0, 0, miniWindowWidth, miniWindowHeight);
			glEnable(GL_SCISSOR_TEST);
			glClearDepth(1.0);
			glClearColor(.5, .5, .5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		}

	//update the projection matrix with the new window properties

	//clear the render buffer


	//update the modelview matrix based on the camera's position
	glMatrixMode(GL_MODELVIEW);              //make sure we aren't changing the projection matrix!
	glLoadIdentity();

	glDrawBuffer(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	//
	//will need to change where we get Our camera and where we are looking at.
	if (viewPortIndex == 0) {
		if (!isFreeCam) {
			switch (currentIndex) {
			case 0:
					gluLookAt(kylesHero->getCamera().getCameraX(), kylesHero->getCamera().getCameraY(), kylesHero->getCamera().getCameraZ(),      // camera is located at (10,10,10)
						kylesHero->getCarX(), kylesHero->getCarY(), kylesHero->getCarZ(),   	// camera is looking at (0,0,0)
						0, 1, 0);
					//std::cout << "Arc Ball!!" << std::endl;
				break;
			case 1:
					gluLookAt(petersHero->getCamera().getCameraX(), petersHero->getCamera().getCameraY(), petersHero->getCamera().getCameraZ(),      // camera is located at (10,10,10)
						petersHero->getCarX(), petersHero->getCarY(), petersHero->getCarZ(),   	// camera is looking at (0,0,0)
						0, 1, 0);
				break;
			case 2:
					gluLookAt(corbinsHero->getCamera().getCameraX(), corbinsHero->getCamera().getCameraY(), corbinsHero->getCamera().getCameraZ(),      // camera is located at (10,10,10)
						corbinsHero->getCarX(), corbinsHero->getCarY(), corbinsHero->getCarZ(),   	// camera is looking at (0,0,0)
						0, 1, 0);
				break;
					
			}
		}
		else {
			gluLookAt(freeCam.getCameraX(), freeCam.getCameraY() , freeCam.getCameraZ(),      // camera is located at (10,10,10)
				(float)freeCam.getCameraX() + (float)freeCam.getDirX(), (float)freeCam.getCameraY() + (float)freeCam.getDirY(), (float)freeCam.getCameraZ() + (float)freeCam.getDirZ(), 	// camera is looking at (0,0,0)
				0, 1, 0);
		
		}


		glCallList(environmentDL);

		kylesHero->draw();
		petersHero->draw();
		corbinsHero->draw();

		particleSystem.draw();
	}
		
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glRasterPos2f(.5, -1);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	char buf[300];
	char fpsArray[200];
	sprintf(fpsArray, "%f", updateFps);
	sprintf(buf, fpsArray);
	const char * p = buf;
	do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p); while (*(++p));

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	if (viewPortIndex == 1 && firstPersonCam) {
		switch (currentIndex) {
		case 0:
			if (firstPersonCam) {
				gluLookAt(kylesHero->getCarX(), kylesHero->getCarY() + 5, kylesHero->getCarZ(),      // camera is located at (10,10,10)
					kylesHero->getCarX() + kylesHero->getFirstPersonCamera().getDirX(), kylesHero->getCarY() + 5 + kylesHero->getFirstPersonCamera().getDirY(), kylesHero->getCarZ() + kylesHero->getFirstPersonCamera().getDirZ(),   	// camera is looking at (0,0,0)
					0, 1, 0);
				//kylesHero->getCarX() + kylesHero->getFirstPersonCamera().getDirX(), kylesHero->getCarY() + kylesHero->getFirstPersonCamera().getDirY(), kylesHero->getCarZ() + kylesHero->getFirstPersonCamera().getDirZ(),   	// camera is looking at (0,0,0)
				//std::cout << "First Person!!" << std::endl;
				//std::cout << "Current x: " << kylesHero->getFirstPersonCamera().getCameraX() << "Current y: " << kylesHero->getFirstPersonCamera().getCameraY() << "Current z: " << kylesHero->getFirstPersonCamera().getCameraZ() << std::endl;
				//std::cout << "Dir x: " << kylesHero->getFirstPersonCamera().getDirX() << "Dir y: " << kylesHero->getFirstPersonCamera().getDirY() << "Dir z: " << kylesHero->getFirstPersonCamera().getDirZ() << std::endl;
				//std::cout << "look at x: " << (float)kylesHero->getFirstPersonCamera().getCameraX() + (float)kylesHero->getFirstPersonCamera().getDirX() << "look at y: " << (float)kylesHero->getFirstPersonCamera().getCameraY() + (float)kylesHero->getFirstPersonCamera().getDirY() << "look at z: " << (float)kylesHero->getFirstPersonCamera().getCameraZ() + (float)kylesHero->getFirstPersonCamera().getDirZ() << std::endl;
			}
			break;
		case 1:
			if (firstPersonCam) {
				gluLookAt(petersHero->getCarX(), petersHero->getCarY() + 5, petersHero->getCarZ(),      // camera is located at (10,10,10)
					petersHero->getCarX() + petersHero->getFirstPersonCamera().getDirX(), petersHero->getCarY() + 5 + petersHero->getFirstPersonCamera().getDirY(), petersHero->getCarZ() + petersHero->getFirstPersonCamera().getDirZ(),   	// camera is looking at (0,0,0)
					0, 1, 0);
			}
			break;
		case 2:
			if (firstPersonCam) {
				gluLookAt(corbinsHero->getCarX(), corbinsHero->getCarY() + 5, corbinsHero->getCarZ(),      // camera is located at (10,10,10)
					corbinsHero->getCarX() + corbinsHero->getFirstPersonCamera().getDirX(), corbinsHero->getCarY() + 5 + corbinsHero->getFirstPersonCamera().getDirY(), corbinsHero->getCarZ() + corbinsHero->getFirstPersonCamera().getDirZ(),   	// camera is looking at (0,0,0)
					0, 1, 0);
			}
			break;

		}
	



	glCallList(environmentDL);
	
	kylesHero->draw();
	petersHero->draw();
	corbinsHero->draw();
	for (unsigned int i = 0; i < heroes.size(); i++) {
		//heroes[i]->draw();
		//std::cout << "Hero " << i + 1 << " x: " << heroes[i]->getCarX() << std::endl;
		//currentHero->draw();
	}
	}
	glFlush();
	}
		//push the back buffer to the screen
	glutSwapBuffers();
}
//
//
// normalKeysDown() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user presses a key.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysDown(unsigned char key, int x, int y) {
	if (key == 'q' || key == 'Q' || key == 27)
		exit(0);
	if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
		if (!isFreeCam) {
			kylesHero->setMoving(true);
			if (key == 'w') {
				kylesHero->moveForward();
			}
			if (key == 's') {
				kylesHero->moveBackward();
			}
			if (key == 'd') {
				kylesHero->turnRight();
			}
			if (key == 'a') {
				kylesHero->turnLeft();
			}
		}
		else {
			if (key == 'w') {
				freeCam.moveFCCamera(true);
			}
			if (key == 's') {
				freeCam.moveFCCamera(false);
			}

		}

	}
	else {
		kylesHero->setMoving(false);
	}
	/*
	if (key == 'f') {
		std::cout << "f key pressed" << std::endl;
		if (isFreeCam) {
			isFreeCam = false;
			firstPersonCam = true;
			//std::cout << "First Person Cam" << std::endl;
		} else if (firstPersonCam) {
			firstPersonCam = false;
			//std::cout << "Arc Ball" << std::endl;
		}
		else {
			isFreeCam = true;
			//std::cout << "Free Cam" << std::endl;
		}

	}*/
	/*
	std::cout << "Start: ----" << std::endl;
	std::cout << "Before: " << kylesHero.getCamera().getDirX() << " " << kylesHero.getCamera().getDirY() << " " << kylesHero.getCamera().getDirZ() << std::endl;
	kylesHero.getCamera().recomputeOrientation(kylesHero.getCarTheta());
	std::cout << "After: " << kylesHero.getCamera().getDirX() << " " << kylesHero.getCamera().getDirY() << " " << kylesHero.getCamera().getDirZ() << std::endl;
	std::cout << "This is what the movement should be: " << kylesHero.getCamera().getMovementX() << " " << kylesHero.getCamera().getMovementZ() << std::endl;
	std::cout << "End: ----" << std::endl;
	*/
	kylesHero->updateCamera();
	kylesHero->updateFirstPersonCamera();
	glutPostRedisplay();
	//
}

void normalKeysUp(unsigned char key, int x, int y) {
	kylesHero->setMoving(false);
}

void printControlPoints(vector<Point> controlPoints) {
	std::cout << "Number of control points: " << controlPoints.size() << std::endl;
	for (unsigned int i = 0; i < controlPoints.size(); i++) {
		std::cout << "x: " << controlPoints[i].getX() << " y: " << controlPoints[i].getY() << " z: " << controlPoints[i].getZ() << std::endl;
	}
	std::cout << "----------------" << std::endl;
}

bool loadControlPoints(char* filename) {

	ifstream myfile(filename);
	if (!myfile.is_open()) {
		std::cout << "The file couldn't be opened." << std::endl;
		exit(-1);
	}
	else {
		//myfile >> number;
		//int bezierSurfaceNumber;
		//myfile >> bezierSurfaceNumber;
		std::string line;
		std::getline(myfile, line);
		//std::cout<<line<<std::endl;
		int bezierSurfaceNumber = atof(line.c_str()) * 16;
		//std::cout << bezierSurfaceNumber << std::endl;
		for (int i = 0; i <= bezierSurfaceNumber; i++)
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
			//std::cout << test[0] << std::endl;
			if (i != 0) {
				//std::cout << test[0] << " "<< test[1] << " " << test[2] << std::endl;
				//Point tester;
				//std::cout << tester.getX() << std::endl;
				Point temp(test[0], test[1], test[2]);
				bezierSurfaceControlPoints.push_back(temp);
			}
			
		}
		std::string temp;
		std::getline(myfile, temp);
		//std::cout << temp << std::endl;
		//myfile >> number;
		int number = atof(temp.c_str());
		//myfile >> number;
		//std::cout << number << std::endl;
		
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
			//std::cout << test[0] << std::endl;
			if (i != 0) {
				//std::cout << test[0] << " "<< test[1] << " " << test[2] << std::endl;
				//Point tester;
				//std::cout << tester.getX() << std::endl;
				Point temp(test[0], test[1], test[2]);
				trackControlPoints.push_back(temp);
			}
		}

		std::string object;
		std::getline(myfile, object);
		//std::cout << temp << std::endl;
		//myfile >> number;
		int numObjects = atof(object.c_str());
		//myfile >> number;
		//std::cout << number << std::endl;

		for (int i = 0; i <= numObjects; i++)
		{
			vector<string> result;
			std::string line;
			std::getline(myfile, line);
			//std::cout<<line<<std::endl;
			std::stringstream lineStream(line);
			std::string cell;
			vector<float> test;
			std::string name;
			int a = 0;
			while (std::getline(lineStream, cell, ','))
			{
				if (a == 0) {
					name = cell;
				}
				test.push_back(atof(cell.c_str()));
				a++;
			}
			//std::cout << test[0] << std::endl;
			if (i != 0) {
				//std::cout << test[0] << " "<< test[1] << " " << test[2] << std::endl;
				//Point tester;
				//std::cout << tester.getX() << std::endl;
				//name 
				Point location(test[1], test[2], test[3]);
				Point orientation(test[4], test[5], test[6]);
				int size = test[7];
				//std::cout << "It's working" << std::endl;
				Shape sh(name, location, orientation,size);
				//std::cout << "It's still working" << std::endl;
				objects.push_back(sh);
				
				
				//trackControlPoints.push_back(temp);
				//std::cout << "Name: " << name << std::endl;
				//std::cout << "Location: " << location.getX() << "Location: " << location.getY() << "Location: " << location.getZ() << std::endl;
				//std::cout << "Orientation: " << orientation.getX() << std::endl;
				//std::cout << "Size: " << size << std::endl;
			}
		}
		//printControlPoints(bezierSurfaceControlPoints);
		//printControlPoints(trackControlPoints);
		petersHero->loadTrackControlPoints(trackControlPoints);
		corbinsHero->loadTrackControlPoints(trackControlPoints);
		
		//printControlPoints(bezierSurfaceControlPoints);
	}
	return true;
}
//tes

// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Does not process command line arguments.
//
////////////////////////////////////////////////////////////////////////////////
//Hero *temp;
int main(int argc, char **argv) {
	
	//Hero1 kyle;
	//temp = &kyle;
	//std::cout << temp->getCarX() << std::endl;
	if (argv[1] == NULL || argv[2] != NULL) {
		std::cout << "Error! Incorrect number of arguments. Please use filename.csv as your argument for running my assignment" << std::endl;
		exit(-1);
	}
	loadControlPoints(argv[1]);

	// create a double-buffered GLUT window at (50,50) with predefined windowsize
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("God Squad Midterm Project");

	// give the camera a scenic starting point.
	//cameraX = 60; //60
	//cameraY = 40; //40
	//cameraZ = 30; //30
	mouseX = windowWidth / 2;
	mouseY = windowHeight / 2;
	//carX = 0;
	//carY = .5;
	//carZ = 0;
	totalLength = 78;
	//carRadius = 1;
	//carPhi = 0;
	//carTheta = 0;
	//cameraRadius = 1;
	//cameraTheta = 8.8f;
	//cameraPhi = 2.1f;
	//qobj = gluNewQuadric();
	kylesHero->updateCamera();
	currentHero = kylesHero;
	heroes.push_back(kylesHero);
	heroes.push_back(petersHero);
	heroes.push_back(corbinsHero);
	//std::cout << kylesHero.getCamera().getDirX();
	// register callback functions...
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(normalKeysDown);
	glutKeyboardUpFunc(normalKeysUp);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resizeWindow);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotion);
	glutTimerFunc(milliseconds, myTimer, 0);
	// do some basic OpenGL setup
	initScene();

	createMenus();
	// and enter the GLUT loop, never to exit.
	glutMainLoop();

	//delete kylesHero;
	//delete petersHero;
	//delete corbinsHero;
	return(0);
}

#endif