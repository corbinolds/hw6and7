#ifndef HERO_1
#define HERO_1


#include <string>
#include <vector>
#include "Point.h"
#include "Camera.h"
#include "Hero.h"
using namespace std;


class Hero1 : public Hero {
public:
	string petCurveFileName;
	float carX, carY, carZ;
	float bezHeight;
	float carTheta, carPhi;
	float coneSize;
	float oldTheta;
	float petX, petY, petZ;
	bool moving, coneDecrease, ctrlIsPressed, hideControl, hideBezier, petIncrease;
	float dis, petScale;
	unsigned int controlIndex;
	vector<Point> petControlPoints;
	Camera cam;
	Camera firstPersonCamera;
	Hero1();
	Hero1(string fileName);

	/// Methods needed for each hero
	void draw();
	void animate();
	void moveForward();
	void moveBackward();
	void turnLeft();
	void turnRight();
	void setMoving(bool moving);
	Camera getCamera();
	Camera getFirstPersonCamera();
	void updateCamera();
	void updateFirstPersonCamera();
	void moveCamera(int totalLength);
	void updateCameraPhi(float dy);
	void updateCameraTheta(float dx);
	void updateFPCameraPhi(float dy);
	void updateFPCameraTheta(float dx);
	void updateCarY(float dy);
	/////
	float getCarTheta();
	float getCarPhi();
	float getCarX();
	float getCarY();
	float getCarZ();
	float getBezH();
private:
	void loadControlPointsForPet();
	void drawCar();
	void drawBezier();
	void drawPet();
	void drawPetBody();
	void drawPetArms();
	void drawPetLegs();
	void drawPetMouth();
	void drawPetEyes();
	void drawPetRightArm();
	void drawPetLeftArm();
	void drawPetRightEye();
	void drawPetLeftEye();
	void drawCurve();
	void drawBox();
	void drawPoints();
	void drawTop();
	void drawWheels();
	void renderBezierCurve(Point point1, Point point2, Point point3, Point point4, int dis);
	Point evaluateBezierCurve(Point point1, Point point2, Point point3, Point point4, float t);
	void drawBody();
	void drawWheelsAndAxel();
	void drawWheel();
	void drawText(char*name);
};


#endif