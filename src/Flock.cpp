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
