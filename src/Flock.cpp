#include "Flock.h"

using namespace std;

Flock::Flock() 
{
	//srand(time(NULL));
	// TODO: init boundaries
}

void Flock::addBoid(Boid b)
{
	boids.push_back(b);
}

void Flock::run()
{
	for (vector<Boid>::iterator it = boids.begin() ; it != boids.end() ; ++it) {
		it->run(boids.begin(), boids.end());
	}
}