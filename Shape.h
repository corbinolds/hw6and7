#ifndef SHAPES
#define SHAPES

#include "Point.h"
#include <string>
using namespace std;

class Shape {
public:
	Shape(string name, Point location, Point orientation, int size);
	string getName();
	Point getLocation();
	Point getOrientation();
	int getSize();
private:
	string name;
	Point location;
	Point orientation;
	int size;
};

#endif
