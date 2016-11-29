#include "Boids.h"

using namespace std;

Boids::Boids() 
{
	//srand(time(NULL));
	// TODO: init boundaries
}

Boids::Boids(int flockSize) 
{
	Boids();
	
	for(int i = 0 ; i < flockSize ; i++)
		add(vec3(randFloat(lowBound.x,highBound.x),
				 randFloat(lowBound.y,highBound.y),
				 randFloat(lowBound.z,highBound.z)
				 ));


	// TODO: init boundaries
}

float Boids::min(float a, float b)
{
	if(a < b)
		return a;
	else 
		return b;
}

float Boids::max(float a, float b)
{
	if(a > b)
		return a;
	else 
		return b;
}

float Boids::randFloat(float min, float max) {
	float range = max - min;
	float num = range * rand() / RAND_MAX;
	return (num + min);
}

void Boids::add(vec3 position)
{
	flockSize++;

	Boiden b;
	b.position = position;
	b.velocity = vec3(0,0,0);
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
	vector<Boiden>::iterator it;
	int i;
	
	for(it = boids.begin(), i = 0 ; it != boids.end() ; ++it, ++i)
	{
		 cout << "boiden" << i << ": ";
		 printVec3((*it).position, "p");
		 printVec3((*it).velocity, "v");
		 cout << endl;
	}
}
void Boids::update()
{
	vector<Boiden>::iterator it;
	
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
vector<Boiden>::iterator Boids::begin()
{
	return boids.begin();
}
vector<Boiden>::iterator Boids::end()
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