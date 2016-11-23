
#include "Camera.h"
#include <math.h>
#include <stdio.h>
#include <iostream>	
using namespace std;

Camera::Camera() {
  cameraRadius = 1;
  cameraTheta = 8.8f;
  cameraPhi = 2.1f;
  cameraX = 60; //60
  cameraY = 40; //40
  cameraZ = 30; //30
  cameraSpeed = .5;
  dirX = cos(cameraTheta);
  dirY = -sin(cameraPhi);
  dirZ = sin(cameraTheta);
}

Camera::Camera(int x, int y, int z, float theta, float phi) {
	cameraRadius = 1;
	cameraTheta = theta;
	cameraPhi = phi;
	cameraX = x; //60
	cameraY = y; //40
	cameraZ = z; //30
	cameraSpeed = .5;
  dirX = cos(cameraTheta);
  dirY = -sin(cameraPhi);
  dirZ = sin(cameraTheta);
}


void Camera::moveCamera(float totalLength, float carX, float carY, float carZ) {
  cameraRadius = totalLength;
  cameraX = -cameraRadius * cos(cameraTheta) * sin(cameraPhi);
  cameraZ = cameraRadius * sin(cameraTheta) * sin(cameraPhi);
  cameraY = -cameraRadius * cos(cameraPhi);
  cameraX = cameraX + carX;
  cameraY = cameraY + carY;
  cameraZ = cameraZ + carZ;
}
void Camera::moveFCCamera(bool forward) {
	if (forward) {
		cameraX = cameraX + cameraSpeed * dirX;
		cameraY = cameraY + cameraSpeed * dirY;
		cameraZ = cameraZ + cameraSpeed * dirZ;
	}
	else {
		cameraX = cameraX - cameraSpeed * dirX;
		cameraY = cameraY - cameraSpeed * dirY;
		cameraZ = cameraZ - cameraSpeed * dirZ;
	}
}

void Camera::recomputeOrientation(float carTheta) {
  // TODO #5: Convert spherical coordinates into a cartesian vector
// see Wednesday's slides for equations.  Extra Hint: Slide #34

  dirX = cos(carTheta);
  dirZ = sin(carTheta);
  //dirZ = sin(carTheta);
  //dirX = cos(carTheta);
  dirY = cos(cameraPhi);
  float length = sqrt(pow(dirX,2) + pow(dirY,2) + pow(dirZ,2));
  //std::cout << "length: " << length << std::endl;
  // and NORMALIZE this directional vector!!!
  dirX = dirX / length;
  dirY = dirY / length;
  dirZ = dirZ / length;
  //std::cout << "During dir x: " << dirX << std::endl;
  //std::cout << "During dir y: " << dirY << std::endl;
  //std::cout << "During dir z: " << dirZ << std::endl;

}
void Camera::recomputeFCOrientation() {
	// TODO #5: Convert spherical coordinates into a cartesian vector
	// see Wednesday's slides for equations.  Extra Hint: Slide #34

	dirX = -cameraRadius * cos(cameraTheta) * sin(cameraPhi);
	dirZ = cameraRadius * sin(cameraTheta) * sin(cameraPhi);
	dirY = -cameraRadius * cos(cameraPhi);
	float length = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));
	//std::cout << "length: " << length << std::endl;
	// and NORMALIZE this directional vector!!!
	dirX = dirX / length;
	dirY = dirY / length;
	dirZ = dirZ / length;
	//std::cout << "During dir x: " << dirX << std::endl;
	//std::cout << "During dir y: " << dirY << std::endl;
	//std::cout << "During dir z: " << dirZ << std::endl;

}

void Camera::recomputeFPOrientation(float carTheta) {
  // TODO #5: Convert spherical coordinates into a cartesian vector
  // see Wednesday's slides for equations.  Extra Hint: Slide #34

  dirX = -cameraRadius * cos(cameraTheta) * sin(cameraPhi) + cos(carTheta);
  dirZ = cameraRadius * sin(cameraTheta) * sin(cameraPhi) + sin(carTheta);;
  dirY = -cameraRadius * cos(cameraPhi);
  float length = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));
  //std::cout << "length: " << length << std::endl;
  // and NORMALIZE this directional vector!!!
  dirX = dirX / length;
  dirY = dirY / length;
  dirZ = dirZ / length;
  //std::cout << "During dir x: " << dirX << std::endl;
  //std::cout << "During dir y: " << dirY << std::endl;
  //std::cout << "During dir z: " << dirZ << std::endl;

}

void Camera::updateCameraTheta(float dx) {
  cameraTheta += dx;
}

void Camera::updateFCCameraTheta(float dx) {
	cameraTheta -= dx;
}

void Camera::updateCameraPhi(float dy) {
  cameraPhi -= dy;

  if (cameraPhi < M_PI / 2) {
    cameraPhi = (M_PI / 2) + .001;
  }
  if (cameraPhi > M_PI) {
    cameraPhi = M_PI - .001;
  }
}
void Camera::updateFCCameraPhi(float dy) {
	cameraPhi -= dy;
	if (cameraPhi < 0) {
		cameraPhi = (0) + .001;
	}
	if (cameraPhi > M_PI) {
		cameraPhi = M_PI - .001;
	}
}

float Camera::getCameraX() {
  return cameraX;
}

float Camera::getCameraY() {
  return cameraY;
}

float Camera::getCameraZ() {
  return cameraZ;
}
float Camera::getCameraSpeed() {
  return cameraSpeed;
}
float Camera::getMovementX() {
  return cameraSpeed * dirX;
}
float Camera::getMovementZ() {
  return cameraSpeed * dirZ;
}

float Camera::updateCameraDecreaseX(float x) {
    return cameraX -= x;
}
float Camera::updateCameraDecreaseZ(float z) {
    return cameraZ -= z;
}
float Camera::updateCameraIncreaseX(float x) {
  return cameraX += x;
}
float Camera::updateCameraIncreaseZ(float z) {
  return cameraZ += z;
}

float Camera::getDirX() {
	return dirX;
}

float Camera::getDirY() {
	return dirY;
}

float Camera::getDirZ() {
	return dirZ;
}
