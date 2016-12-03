
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Boid
{
public:
	Boid(vec3 p, vec3 v);
	void run(vector<Boid>::iterator start, vector<Boid>::iterator end, vec3 boundMin, vec3 boundMax);
	vec3 position;
	vec3 velocity;
	vec3 acceleration;
	float radius;
	float maxForce;
	float minSpeed;
	float maxSpeed;

private:
	vec3 seperation(vector<Boid>::iterator start, vector<Boid>::iterator end);
	vec3 align(vector<Boid>::iterator start, vector<Boid>::iterator end);
	vec3 cohesion (vector<Boid>::iterator start, vector<Boid>::iterator end);
	vec3 seek(vec3 target);
	vec3 wallAvoidance(vec3 min, vec3 max);
};

