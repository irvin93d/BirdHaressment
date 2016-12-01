
#include <vector>
#include <iostream>
#include <string>

#include "Boid.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;


class Flock
{
public:
	Flock();
	//Flock(unsigned int flockSize);
	void addBoid(Boid b);
	void removeBoid();
	void run();
	vector<Boid> boids;
private:
	vec3 boundMin = vec3(-30, 4,-30);
	vec3 boundMax = vec3(30,40,30);
};

