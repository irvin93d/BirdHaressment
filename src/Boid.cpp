#include "Boid.h"

Boid::Boid(vec3 p, vec3 v) 
{
    position = p;
    velocity = v;
	radius = 1.0;
	maxSpeed = 0.01;
	maxForce = 0.003;
}

void Boid::run(vector<Boid>::iterator start, vector<Boid>::iterator end)
{
	acceleration = vec3(0,0,0);

	// Calculate serparation and add to acceleration (multiply weight)
	vec3 sep = seperation(start, end);
	sep *= 2;
	acceleration += sep;
	// Calculate alignment and add to acceleration (multiply weight)
	vec3 ali = align(start,end);
	ali *= 0.1;
	acceleration += ali;
	// Calculate cohesion and add to acceleration (multiply weight)
	//vec3 coh = cohesion(start,end);
	//coh *= 1;
	//acceleration += coh;


	velocity += acceleration;

	float speed = length(velocity);
	if(speed > maxSpeed)
		velocity *= maxSpeed/speed;
	
	//velocity = vec3(0,0,-0.01);
	position += velocity;
}

vec3 Boid::seperation(vector<Boid>::iterator start, vector<Boid>::iterator end)
{
	float desiredseparation = 50.0;
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
	if (count > 0)
		steer /= (float) count;
    
	// As long as the vector is greater than 0
	float d = length(steer);
	if ( d > 0) 
	{
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

vec3 Boid::align(vector<Boid>::iterator start, vector<Boid>::iterator end) 
{
	float neighbordist = 0.1;
	vec3 sum = vec3(0,0,0);
    int count = 0;
	for (vector<Boid>::iterator it = start ; it != end ; ++it) 
	{
		vec3 diff = position - it->position;
		float d = length(diff);
		if (d > 0 && d < neighbordist) 
		{
			sum += it->velocity;
			count++;
		}
	}
	if (count > 0)
	{
		sum /= (float) count;
		float d = length(sum);
		// First two lines of code below could be condensed with new PVector setMag() method
		// Not using this method until Processing.js catches up
		// sum.setMag(maxspeed);

		// Implement Reynolds: Steering = Desired - Velocity
		sum /= d;
		sum *= maxSpeed;
		vec3 steer = sum - velocity;

		d = length(sum);	
		if(d > maxForce)
			steer *= maxForce/d;
		return steer;
	} 
	else
	{
		return vec3(0,0,0);
	}
}
/*
// For the average position (i.e. center) of all nearby boids, calculate steering vector towards that position
  PVector cohesion (ArrayList<Boid> boids) {
    float neighbordist = 50;
    PVector sum = new PVector(0, 0);   // Start with empty vector to accumulate all positions
    int count = 0;
    for (Boid other : boids) {
      float d = PVector.dist(position, other.position);
      if ((d > 0) && (d < neighbordist)) {
        sum.add(other.position); // Add position
        count++;
      }
    }
    if (count > 0) {
      sum.div(count);
      return seek(sum);  // Steer towards the position
    } 
    else {
      return new PVector(0, 0);
    }
  }*/