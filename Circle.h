#pragma once
class Circle
{
public: 
	struct vec2 {
		float v[2];
		vec2 operator- (vec2 a) {
			return { v[0] - a.v[0], v[1] - a.v[1]};
		}
		float operator[] (int a) {
			return v[a];
		}
	};
	struct vertexs {
		float v[6];
	};
	
	/* Constructor */
	Circle(vec2 radius, vec2 centerPos);
	vec2 getRadius();
	vec2 getCenter();
	vertexs getVertx();
	void setRadius(vec2);
	void setCenter(vec2);
	void setVelocity(vec2);
	vec2 getVelocity();
	void move(float time);
	bool checkWallColision(float time);
	bool checkCircleColision(Circle circle, float time);

private:
	vec2 rotate(vec2 A, float angle);
	float q_rsqrt(float number);
	vec2 center;
	vec2 radius; // index 0 represents outer radius \n index 1 represents inner radius
	vec2 velocity;
};
