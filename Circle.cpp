#include "Circle.h"
#include "cmath"

#define PI acosf(-1) 

Circle::Circle(vec2 radius, vec2 center) 
{
	this->radius = radius;
	this->center = center;
	this->velocity = {0};
}

Circle::vec2 Circle::getCenter() {
	return this->center;
}

Circle::vec2 Circle::getRadius() {
	return this->radius;
}

void Circle::setCenter(Circle::vec2 c)
{
	this->center = c;
}

void Circle::setRadius(vec2 r) {
	this->radius = r;
}

Circle::vertexs Circle::getVertx() {
	return Circle::vertexs{
		{	this->center.v[0] + radius.v[0] * 1.75f, this->center.v[1] - this->radius.v[0],
			this->center.v[0] - radius.v[0] * 1.75f, this->center.v[1] - this->radius.v[0],
			this->center.v[0], this->center.v[1] + 2 * this->radius.v[0],
		}
	};
}

Circle::vec2 Circle::getVelocity() {
	return this->velocity;
}

void Circle::setVelocity(vec2 v) {
	this->velocity = v;
}

void Circle::move(float time) {
	this->center = {
		this->center.v[0] + time * this->velocity.v[0],
		this->center.v[1] + time * this->velocity.v[1]
	};
}

bool Circle::checkWallColision(float time) {
	if (center.v[0] > (1 - radius.v[0]) || center.v[0] < (radius.v[0] - 1)) {
		velocity.v[0] = -velocity.v[0];
		move(time);
		return true;
	}
	if (center.v[1] > (1 - radius.v[0]) || center.v[1] < (radius.v[0] - 1)) {
		velocity.v[1] = -velocity.v[1];
		move(time);
		return true;
	}
	return false;
}

bool Circle::checkCircleColision(Circle Circle, float time) {
	vec2 absCenter = this->center - Circle.getCenter();
	float distanceSquared = (absCenter.v[0] * absCenter.v[0]) + (absCenter.v[1] * absCenter.v[1]);
	if (!((std::powf(Circle.getRadius()[1] - this->radius[0], 2) > distanceSquared) &&
		std::powf(Circle.getRadius()[0] + this->radius[0], 2 < distanceSquared)))
	{
		vec2 tangent = {
			-absCenter[1] * q_rsqrt(powf(absCenter[0],2) + powf(absCenter[1], 2)),
			absCenter[0] * q_rsqrt(powf(absCenter[0],2) + powf(absCenter[1], 2))
		};
		vec2 normalizedVelocity = {
			velocity[0] * q_rsqrt(powf(velocity[0], 2) + powf(velocity[1], 2)),
			velocity[1] * q_rsqrt(powf(velocity[0], 2) + powf(velocity[1], 2))
		};
		float theta = acosf( (normalizedVelocity[0] * tangent[0] + normalizedVelocity[1] * tangent[1]) ); // V dot T = |V||T| *cos(theta)
		if ( theta < PI / 2){
			if ( rotate(normalizedVelocity, theta)[0] == tangent[0] ) { // rotate left once if its the same v is on the right of tan
				velocity = rotate(velocity, -2 * theta);
			}
			else {
				velocity = rotate(velocity, 2 * theta);
			}
		}
		else {
			if (rotate(normalizedVelocity, PI - theta)[0] == tangent[0]) { // rotate left once if its the same v is on the right of tan
				velocity = rotate(velocity, 2 * (PI - theta) );
			}
			else {
				velocity = rotate(velocity, -2 * (PI - theta) );
			}
		}
		move(time);
		return true;
	}
	else {
		return false;
	}
}

/*Fast inverse square root algorithim from quake 3*/
float Circle::q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

Circle::vec2 Circle::rotate(vec2 A, float angle) {
	return {
		A[0] * cosf(angle) - A[1] * sinf(angle),
		A[0] * sinf(angle) + A[1] * cosf(angle)
	};
}
