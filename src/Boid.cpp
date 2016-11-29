#include "Boid.h"

Boid::Boid(vec3 p, vec3 v) 
{
    position = p;
    velocity = v;
    acceleration = vec3(0,0,0);
	radius = 1.0;
	maxSpeed = 1;
	maxForce = 0.03;
}