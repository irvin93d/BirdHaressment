#include "Flock.h"

using namespace std;

Flock::Flock() 
{
}

void Flock::addBoid(Boid b)
{
	boids.push_back(b);
}

void Flock::removeBoid()
{
	if(!boids.empty())
		boids.pop_back();
}

void Flock::run()
{
	for (vector<Boid>::iterator it = boids.begin() ; it != boids.end() ; ++it) {
		it->run(boids.begin(), boids.end(), boundMin, boundMax);
	}
}