#pragma once
class Circle
{
public: 
	struct vec2 {
		float vec2[2];
	};
	struct vertexs {
		float v[9];
	};
	
	//methods
	Circle(float, vec2);
	float getRadius();
	vec2 getCenter();
	vertexs getVertx();
	void setRadius(float);
	void setCenter(vec2);

private:
	vec2 center;
	float radius;
};

