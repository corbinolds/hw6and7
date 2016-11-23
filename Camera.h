#ifndef CAMERA
#define CAMERA
class Camera {
  float cameraX, cameraY, cameraZ;            // camera position in cartesian coordinates
  float cameraRadius, cameraTheta, cameraPhi, cameraSpeed;
  float dirX, dirY, dirZ;
  public:
    Camera();
	Camera(int x, int y, int z, float theta, float phi);
    void moveCamera(float totalLength, float carX, float carY, float carZ);
	void moveFCCamera(bool forward);
    void recomputeOrientation(float carTheta);
	void recomputeFCOrientation();
    void updateCameraTheta(float dx);
	void updateFCCameraTheta(float dx);
    void updateCameraPhi(float dy);
	void updateFCCameraPhi(float dy);
  void recomputeFPOrientation(float carTheta);
    float getCameraX();
    float getCameraY();
    float getCameraZ();
    float getCameraSpeed();
    float getMovementX();
    float getMovementZ();
    float updateCameraDecreaseX(float x);
    float updateCameraDecreaseZ(float z);
    float updateCameraIncreaseX(float x);
    float updateCameraIncreaseZ(float z);
	float getDirX();
	float getDirY();
	float getDirZ();
  private:

};
#endif