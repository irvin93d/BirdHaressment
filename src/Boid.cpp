#include "Boid.h"

Boid::Boid(vec3 p, vec3 v) 
{
    position = p;
    velocity = v;
	radius = 1.0;
	maxSpeed = 0.01;
	maxForce = 0.03;
}

void Boid::run(vector<Boid>::iterator start, vector<Boid>::iterator end)
{
	acceleration = vec3(0,0,0);

	// Calculate serparation and add to acceleration (multiply weight)
	acceleration += seperation(start, end);
	// Calculate alignment and add to acceleration (multiply weight)
	// Calculate cohesion and add to acceleration (multiply weight)

	velocity += acceleration;

	float speed = length(velocity);
	if(speed > maxSpeed)
		velocity *= maxSpeed/speed;
	
	//velocity = vec3(0,0,-0.01);
	position += velocity;
}

vec3 Boid::seperation(vector<Boid>::iterator start, vector<Boid>::iterator end)
{
	float desiredseparation = 5.0;
	vec3 steer = vec3(0, 0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (vector<Boid>::iterator it = start ; it != end ; ++it) {
		vec3 diff = position - it->position;
		float d = length(diff);
		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if (d > 0 && d < desiredseparation) {
			// Calculate vector pointing away from neighbor
			diff /= d*d;        // Weight by distance
			//steer += diff;
			count++;            // Keep track of how many
		}
	}
	
	// Average -- divide by how many
	if (count > 0) {
		steer /= (float) count;
	}
    
	// As long as the vector is greater than 0
	float d = length(steer);
	if ( d > 0) {
		// First two lines of code below could be condensed with new PVector setMag() method
		// Not using this method until Processing.js catches up
		// steer.setMag(maxspeed);

		// Implement Reynolds: Steering = Desired - Velocity
		steer /= d;
		steer *= maxSpeed;
		steer -= velocity;
		d = length(steer);
		if(d > maxForce)
			steer *= maxForce/d;
	}
    return steer;
}

