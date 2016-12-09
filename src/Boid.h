
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include "Bullets.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Boid
{
public:
	Boid(vec3 p, vec3 v, vec3 ambColor, vec3 diffColor, vec3 specColor);
	void run(vector<Boid>::iterator start, vector<Boid>::iterator end, vec3 boundMin, vec3 boundMax, Bullets* bullets);
	vec3 position;
	vec3 velocity;
	vec3 acceleration;
	float radius;
	float maxForce;
	float minSpeed;
	float maxSpeed;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

private:
	vec3 seperation(vector<Boid>::iterator start, vector<Boid>::iterator end);
	vec3 align(vector<Boid>::iterator start, vector<Boid>::iterator end);
	vec3 cohesion (vector<Boid>::iterator start, vector<Boid>::iterator end);
	vec3 seek(vec3 target);
	vec3 bulletAvoidance(Bullets* bullets);
	vec3 wallAvoidance(vec3 min, vec3 max);
};

