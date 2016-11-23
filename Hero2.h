#ifndef HERO_2
#define HERO_2

#include <string>
#include <vector>
#include "Point.h"
#include "Camera.h"
#include "Hero.h"
using namespace std;

class Hero2 : public Hero {
public:
	float characterX, characterY, characterZ;
	float carYaw, carPitch;
	float transportRotation;
	float wheelRotation;
	float fanRotate;
	float sphereRadius; //the radius of the control points
	float lineThickness;  //variable for line thickness between points
	vector<Point> spiritControlPoints;
	vector<Point> trackControlPoints;
	float trackPointVal;
	float timerValue; //timer value to control pixie
	float trackTimerValue;
	Camera firstPersonCamera;
	string spiritCurveFile; //csv for the little guy
	string trackCurveFile; //csv for closed track
	char charName[];
	bool DRAW_CAGE;  //Menu option for drawing the bezier points
	bool DRAW_CURVE;  //Menu option for drawing the bezier curve
	bool CTRL_PRESSED;
	bool WHICH_POINTS;
	bool WHICH_POINTS_TRACK;
	Point onBez;
	Point onTrack;
	Point previousTrack;
	Point onTrackNormal;
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
	Hero2();
	Hero2(string filename);
private:
	//Methods Needed to Draw Hero
	void drawCylinder();
	void drawFan();
	void drawCopter();
	void drawWheels();
	void drawBody();
	void drawCharacter();
	void drawText(char*string);
	void drawBezSphere(float x,float y, float z);
	//Methods Needed to Draw Spirit
	void drawSolidSphere();
	void drawSpirit(float inX, float inY, float inZ);
	//Bezier Curve Methods Needed
	Point evaluateTangent(Point p0, Point p1, Point p2, Point p3, float t);
	Point evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3, float t);
	float evaluatePitch(Point a, Point b, Point c);
	float evaluateYaw(Point a, Point b, Point c);
	void renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution);
	void drawSphere(float inX, float inY, float inZ);
	void loadPetControlPoints();
	void drawPetControlPoints();

	void drawTrackControlPoints();
};

#endif
