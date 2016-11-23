#ifndef HERO_3
#define HERO_3

#include <string>
#include <vector>
#include "Point.h"
#include "Camera.h"
#include "Hero.h"
using namespace std;

class Hero3 : public Hero {
public:
	float characterX, characterY, characterZ;
	float transportRotation;
	float wheelRotation;
	float fanRotate;
	float sphereRadius; //the radius of the control points
	float lineThickness;  //variable for line thickness between points
	vector<Point> trackControlPoints;
	vector<float> arcLengthValues;
	vector<Point> bezierPoints;
	float tVal;
	float arcDistance;
	float trackPointVal;
	float timerValue; //timer value to control pixie
	float trackTimerValue;
	Camera firstPersonCamera;
	string trackCurveFile; //csv for closed track
	
	bool WHICH_CURVE;
	bool DRAW_CAGE;  //Menu option for drawing the bezier points
	bool DRAW_CURVE;  //Menu option for drawing the bezier curve
	bool CTRL_PRESSED;
	bool WHICH_POINTS;
	bool WHICH_POINTS_TRACK;
	Point onBez;
	Point onTrack;
	Camera cam;
	//Methods Needed From Abstract Class
	void draw();
	void animate();
	void moveForward();
	float getCarX();
	float getCarY();
	float getCarZ();
	Camera getCamera();
	Camera getFirstPersonCamera();
	void updateCamera();
	void updateFirstPersonCamera();
	void moveCamera(int totalLength);
	void updateCameraPhi(float dy);
	void updateCameraTheta(float dx);
	void updateFPCameraPhi(float dy);
	void updateFPCameraTheta(float dx);
	void loadTrackControlPoints(vector<Point> cP);
	//Constructors
	Hero3();
	Hero3(string filename);
private:
	//Methods Needed to Draw Hero
	void drawCylinder();
	void drawFan();
	void drawCopter();
	void drawWheels();
	void drawBody();
	void drawCharacter();
	void drawText(char* string);
	//Bezier Curve Methods Needed
	Point evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3, float t);
	void renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution);
	void drawSphere(float inX, float inY, float inZ);
	void drawTrackControlPoints();
	float calcTVal();
	float calcLerp(float t1, float t2, float distance, float t1Dis, float t2Dis);
};

#endif
