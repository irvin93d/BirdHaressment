#include "Boid.h"

Boid::Boid(vec3 p, vec3 v) 
{
    position = p;
    velocity = v;
	radius = 0.1;
	minSpeed = 0.1;
	maxSpeed = 0.40;
	maxForce = 0.0015;
}

void Boid::run(vector<Boid>::iterator start, vector<Boid>::iterator end, vec3 boundMin, vec3 boundMax)
{
	acceleration = vec3(0,0,0);

	// Calculate serparation and add to acceleration (multiply weight)
	vec3 sep = seperation(start, end);
	sep *= 1.5;
	acceleration += sep;
	// Calculate alignment and add to acceleration (multiply weight)
	vec3 ali = align(start,end);
	ali *= 1;
	acceleration += ali;
	// Calculate cohesion and add to acceleration (multiply weight)
	vec3 coh = cohesion(start,end);
	coh *= 1;
	acceleration += coh;

	vec3 wav = wallAvoidance(boundMin, boundMax);
	wav *= 1.5;
	acceleration += wav;
	//cout << "acceleration: " << acceleration[0] << "," << acceleration[1] << "," << acceleration[2] << endl;

	float d = length(acceleration);
	if(d > maxForce)
		acceleration *= maxForce/d;
	
	velocity += acceleration;

	float speed = length(velocity);
	if(speed < minSpeed)
		velocity *= minSpeed/speed;
	
	position += velocity;
}

vec3 Boid::seperation(vector<Boid>::iterator start, vector<Boid>::iterator end)
{
	float desiredseparation = 0.5;
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
    
	//cout << steer.x << endl;
	// As long as the vector is greater than 0
	float d = length(steer);
	if ( d > 0) 
	{
		// First two line(s of code below could be condensed with new PVector setMag() method
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
	float neighbordist = 5;
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

	// Above two lines of code below could be condensed with new PVector setMag() method
	// Not using this method until Processing.js catches up
	// desired.setMag(maxspeed);

	// Steering = Desired minus Velocity
	vec3 steer = desired - velocity;
	float d = length(steer);	
	if(d > maxForce)
		steer *= maxForce/d;
	return steer;
}

vec3 Boid::wallAvoidance(vec3 min, vec3 max)
{
	short minmax = 0;
	short xyz = -1;
	float prefered = 5;
	float d = prefered;
	vec3 minBound = position - min;
	vec3 maxBound = max - position;
	//cout << "minBound: " << minBound[0] << "," << minBound[1] << "," << minBound[2] << endl;
	for(int i = 0 ; i < 3 ; i++)
	{
		if(minBound[i] < d)
		{
			//cout << "its set" << endl;
			d = minBound[i];
			minmax = -1;
			xyz = i;	
		}
		if(maxBound[i] < d)
		{
			//cout << "its set" << endl;
			d = maxBound[i];
			minmax = 1;
			xyz = i;
		}
	}

	if(minmax == 0)
		return vec3(0,0,0);

	vec3 steer = vec3(0,0,0);
	steer[xyz] = -minmax;
	// As long as the vector is greater than 0
	if ( d > 0) 
	{
		// First two line(s of code below could be condensed with new PVector setMag() method
		// Not using this method until Processing.js catches up
		// steer.setMag(maxspeed);

		// Implement Reynolds: Steering = Desired - Velocity
		steer *= maxSpeed;
		steer -= velocity;
		d = length(steer);
		if(d > maxForce)
			steer *= maxForce/d;
	}
	//cout << "steer: " << steer[0] << "," << steer[1] << "," << steer[2] << endl;
	//cout << "min: " << min[0] << "," << min[1] << "," << min[2] << endl;
	//cout << "max: " << max[0] << "," << max[1] << "," << max[2] << endl;
    //cout << "position: " << position[0] << "," << position[1] << "," << position[2] << endl;
    return steer;

}
