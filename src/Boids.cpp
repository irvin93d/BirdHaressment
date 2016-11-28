#include "Boids.h"

using namespace std;

Boids::Boids() 
{
	// TODO: init size of flock
	// TODO: init boundaries
	// TODO: create random flock
}

void Boids::add(vec3 position)
{
	flockSize++;

	Boid b;
	b.position = position;
	b.velocity = vec3(position.x /100,0,0);
	boids.push_back(b);

	for(unsigned int i = 0 ; i < flockSize - 1 ; i++)
		distances.push_back(0);
}

//TODO: remove
void Boids::remove(unsigned int i)
{
	if(i < flockSize)
		boids.erase(boids.begin() + i);
}

void Boids::printVec3(vec3 v, string name)
{
	cout << name  << "(" << v[0]
		<< "," << v[1]
		<< "," << v[2]
		<< ") ";
}

//TODO: remove
void Boids::printAll()
{
	vector<Boid>::iterator it;
	int i;
	
	for(it = boids.begin(), i = 0 ; it != boids.end() ; ++it, ++i)
	{
		 cout << "boid" << i << ": ";
		 printVec3((*it).position, "p");
		 printVec3((*it).velocity, "v");
		 cout << endl;
	}
}
void Boids::update()
{
	vector<Boid>::iterator it;
	
	// Set new position
	for(it = boids.begin(); it != boids.end() ; ++it)
	{
		(*it).position += (*it).velocity; 
	}

	// TODO: Calculate all nearest	
	// TODO: Calculate velocity based on nearest
	// TODO: Velocity based on world
	// TODO: aligment stuffd


	// update distances
	for(unsigned int a = 0, b = 1, i = 0; b < flockSize ; a++)
	{
		distances[i++] = length(boids[b].position - boids[a].position);
		if(a >= b-1)
		{
			a = 0;
			b++;
		}
	}

}
vector<Boid>::iterator Boids::begin()
{
	return boids.begin();
}
vector<Boid>::iterator Boids::end()
{
	return boids.end();
}

float Boids::getDistance(unsigned int a, unsigned int b)
{
	//distance to self is 0
	if(a == b)
		return 0;
	// keep high in b
	if(a > b)
	{
		unsigned int c = a;
		a = b;
		b = c;
	}
	unsigned int ind = 0;
	do{
		ind += (--b);
	} while (b > 1); 
	
	return distances[ind + a];
}