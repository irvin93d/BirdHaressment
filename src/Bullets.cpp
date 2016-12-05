#include "Bullets.h"

using namespace std;

Bullets::Bullets() 
{
}

void Bullets::addBullet(vec3 position, vec3 direction)
{
	Bullet b;
	b.pos = position;
	b.dir = direction;
	b.speed = 0.5;
	b.lifetime = 100;
	b.timealive = 0;
	bullets.push_back(b);
}

void Bullets::run()
{
	vector<Bullet>::iterator it = bullets.begin();
	while( it != bullets.end()) {
		vec3 movement = it->speed * it->dir;
		it->pos +=  movement;
		it->timealive += it->speed;

		if(it->timealive >= it->lifetime)
			it = bullets.erase(it);
		else
			++it;
	}
}