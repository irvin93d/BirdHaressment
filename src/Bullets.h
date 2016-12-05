
#ifndef __BULLETS_H__ 
#define __BULLETS_H__ 

#include <vector>
#include <iostream>
#include <string>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;


class Bullet{
public:
	vec3 pos;
	vec3 dir;
	float speed;
	float lifetime;
	float timealive;
};

class Bullets
{
public:
	Bullets();
	void addBullet(vec3 position, vec3 direction);
	void run();
	vector<Bullet> bullets;
};

#endif 
