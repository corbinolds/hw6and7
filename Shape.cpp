#include "Point.h"
#include "Shape.h"
#include <string>
using namespace std;

Shape::Shape(string name, Point location, Point orientation, int size) {
	this->name = name;
	this->location = location;
	this->orientation = orientation;
	this->size = size;
}


string Shape::getName() {
	return name;
}

Point Shape::getLocation() {
	return location;
}

Point Shape::getOrientation() {
	return orientation;
}

int Shape::getSize() {
	return size;
}