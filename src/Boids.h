
#include <sys/time.h>
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <string>

using namespace std;
using namespace glm;

#define MIN_VELOCITY 0.5 // TODO
#define MAX_VELOCITY 5.0 // TODO
#define FLOCK_SIZE 5 // TODO

// flock velocity
// flock center

typedef struct 
{
	vec3 position;
	vec3 velocity;
	// TODO: vec3 acceleration;
} Boid;

class Boids
{
public:
	Boids();
	void update();
	void printAll();
	void add(vec3 position);
	void remove(int i);
	float getDistance(unsigned int a, unsigned int b);
	vector<Boid>::iterator begin();
	vector<Boid>::iterator end();
private:
	void printVec3(vec3 v, string name);
	vector<Boid> boids;
	unsigned int flockSize = 0;
	clock_t lastUpdate;
	vector<float> distances; // {0-1, 0-2, 1-2, 0-3, 1-3, 2-3 ...
};

