
#include <vector>
#include <iostream>
#include <string>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Boid
{
public:
	Boid(vec3 p, vec3 v);
	void run(vector<Boid>::iterator start, vector<Boid>::iterator end);
	vec3 position;
	vec3 velocity;
	vec3 acceleration;
	float radius;
	float maxForce;
	float maxSpeed;

//private:

};

