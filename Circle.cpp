#include "Circle.h"

Circle::Circle(float radius, vec2 center) 
{
	this->radius = radius;
	this->center = center;
}

Circle::vec2 Circle::getCenter() {
	return this->center;
}

float Circle::getRadius() {
	return this->radius;
}

void Circle::setCenter(Circle::vec2 c)
{
	this->center = c;
}

void Circle::setRadius(float r) {
	this->radius = r;
}

Circle::vertexs Circle::getVertx() {
	return Circle::vertexs{
		{	this->center.vec2[0] + radius * 1.75f, this->center.vec2[1] - this->radius, 0,
			this->center.vec2[0] - radius * 1.75f, this->center.vec2[1] - this->radius, 0,
			this->center.vec2[0], this->center.vec2[1] + 2 * this->radius, 0
		}
	};
}