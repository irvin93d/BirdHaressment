#include "Boid.h"

Boid::Boid(vec3 p, vec3 v, vec3 ambColor, vec3 diffColor, vec3 specColor) 
{
    position = p;
    velocity = v;
	radius = 0.1;
	minSpeed = 0.1;
	maxSpeed = 0.40;
	maxForce = 0.0015;
	ambient = ambColor;
	diffuse = diffColor;
	specular = specColor;
}

void Boid::run(vector<Boid>::iterator start, vector<Boid>::iterator end, vec3 boundMin, vec3 boundMax, Bullets* bullets)
{
	acceleration = vec3(0,0,0);

	// Calculate serparation and add to acceleration (multiply weight)
	vec3 sep = seperation(start, end);
	sep *= 2;
	acceleration += sep;
	// Calculate alignment and add to acceleration (multiply weight)
	vec3 ali = align(start,end);
	ali *= 1.5;
	acceleration += ali;
	// Calculate cohesion and add to acceleration (multiply weight)
	vec3 coh = cohesion(start,end);
	coh *= 1.5;
	acceleration += coh;

	vec3 wav = wallAvoidance(boundMin, boundMax);
	wav *= 3;
	acceleration += wav;

	vec3 bav = bulletAvoidance(bullets);
	bav *= 6;
	acceleration += bav;

	velocity += acceleration;

	velocity.y = (velocity.y>0) ? std::min(velocity.y, maxSpeed/3):std::max(velocity.y, -maxSpeed/3);

	position += velocity;
}

vec3 Boid::seperation(vector<Boid>::iterator start, vector<Boid>::iterator end)
{
	float desiredseparation = 3;
	vec3 steer = vec3(0, 0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (vector<Boid>::iterator it = start ; it != end ; ++it) {
		vec3 diff = position - it->position;
		float d = length(diff);
		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if (d > 0 && d < desiredseparation) {
			// Calculate vector pointing away from neighbor
			diff /= d;        // Weight by distance
			steer += diff;
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
	float neighbordist = 5;
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

// For the average position (i.e. center) of all nearby boids, calculate steering vector towards that position
vec3 Boid::cohesion (vector<Boid>::iterator start, vector<Boid>::iterator end) {
	float neighbordist = 15;
	vec3 sum = vec3(0,0,0);   // Start with empty vector to accumulate all positions
	int count = 0;
	for (vector<Boid>::iterator it = start ; it != end ; ++it)
	{
		vec3 diff = position - it->position;
		float d = length(diff);
		if (d > 0 && d < neighbordist)
		{
			sum += it->position; // Add position
			count++;
		}
	}
	if (count > 0)
	{
		sum /= (float)count;
		return seek(sum);  // Steer towards the position
	} 
	else {
		return vec3(0,0,0);
	}
}
vec3 Boid::seek(vec3 target) {
	vec3 desired = target - position;  // A vector pointing from the position to the target
	// Scale to maximum speed
	desired /= length(desired);
	desired *= maxSpeed;

	// Steering = Desired minus Velocity
	vec3 steer = desired - velocity;
	float d = length(steer);	
	if(d > maxForce)
		steer *= maxForce/d;
	return steer;
}

vec3 Boid::wallAvoidance(vec3 min, vec3 max)
{
	float treshold = 2;
	vec3 minDists = position - min;
	vec3 maxDists = max - position;
	vec3 steer = vec3(0,0,0);
	for(int i = 0 ; i < 3 ; i++)
	{
		if(minDists[i] < treshold && minDists[i] < maxDists[i])
			steer[i] = treshold - minDists[i];
		
		else if(maxDists[i] < treshold)
			steer[i] = maxDists[i] - treshold;
	}

	float d = length(steer);
	if ( d > 0) 
	{
		steer /= d;
		steer *= maxSpeed;
		steer -= velocity;
		d = length(steer);
		if(d > maxForce)
			steer *= maxForce/d;
	}
	return steer;
}

vec3 Boid::bulletAvoidance(Bullets* bullets){

	float treshold = 10;
	vec3 steer = vec3(0,0,0);
	for(vector<Bullet>::iterator it = bullets->bullets.begin() ; it != bullets->bullets.end() ; ++it){	
		vec3 toBullet = position - it->pos;
		float d = length(toBullet);
		if(d < treshold){
			steer += (treshold - d)*normalize(cross(it->dir,cross(toBullet, it->dir)));
		}
	}

	float d = length(steer);
	if ( d > 0) 
	{
		steer /= d;
		steer *= maxSpeed;
		steer -= velocity;
		d = length(steer);
		if(d > maxForce)
			steer *= maxForce/d;
	}
	return steer;
}
