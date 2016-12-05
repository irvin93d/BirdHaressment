
#include <vector>
#include <iostream>
#include <string>

#include "Boid.h"
#include "Bullets.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;


class Flock
{
public:
	Flock();
	void addBoid(Boid b);
	void removeBoid();
	void run();
	vector<Boid> boids;
	Bullets* bullets;
private:
	vec3 boundMin = vec3(-30, 3,-30);
	vec3 boundMax = vec3(30,30,30);
};

