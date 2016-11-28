/* Lab 6 base code - transforms using matrix stack built on glm 
	CPE 471 Cal Poly Z. Wood + S. Sueda
*/
#include <iostream>
#define GLEW_STATIC
#define WORLD_SIZE 15
#define WORLD_TILE_SIZE 8
#define OBJECT_SPREAD 30

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Boids.h"


// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

/* to use glee */
#define GLEE_OVERWRITE_GL_FUNCTIONS
#include "glee.hpp"

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from
shared_ptr<Program> phung;
shared_ptr<Program> groundShad;
shared_ptr<Shape> half_pyramid;
shared_ptr<Shape> cube;
shared_ptr<Shape> bunny;
shared_ptr<Shape> sphere;
shared_ptr<Shape> cylinder;
shared_ptr<Shape> cone;
vec3 eye = vec3(2,2,5);
vec3 look = vec3(0,0,0);
vec3 up = vec3(0,1,0);
vec3 lightPos = vec3(20,15,0);
vector<float> bunnyPlacement;
vector<float> treePlacement;
Boids* boids;
vec3 eyeMov = vec3(0,0,0);	

/* Gives a random float between min and max */


int randInt(int min, int max) {
	float range = max - min;
	float num = range * rand() / RAND_MAX;
	return (num + min);
}

static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	if(key == GLFW_KEY_W) {
		if(action == GLFW_PRESS)
			eyeMov.z++;
		else if(action == GLFW_RELEASE)
		{
			eyeMov.z--;
		}
	}
	if(key == GLFW_KEY_S) {
		if(action == GLFW_PRESS)
			eyeMov.z--;
		else if(action == GLFW_RELEASE)
			eyeMov.z++;
	}
	if(key == GLFW_KEY_A) {
		if(action == GLFW_PRESS)
			eyeMov.x--;
		else if(action == GLFW_RELEASE)
			eyeMov.x++;
	}
	if(key == GLFW_KEY_D) {
		if(action == GLFW_PRESS)
			eyeMov.x++;
		else if(action == GLFW_RELEASE)
			eyeMov.x--;
	}
}

static void updateEyePosition()
{
	float moveSpeed = 0.15;
	float rollSpeed = 0.05;
	vec3 w = normalize(eye - look);
	vec3 u = normalize(cross(up, w));
	vec3 v = normalize(cross(w, u));

	w *= moveSpeed*eyeMov.z;
	eye -= vec3(w[0], 0, w[2]);
	look -= vec3(w[0], 0, w[2]);
	
	u *= moveSpeed*eyeMov.x;
	eye += u;
	look += u;
	
}

static void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
   	double posX, posY;
   	if (action == GLFW_PRESS) {
      glfwGetCursorPos(window, &posX, &posY);
      cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
	}
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	float moveSpeed = 0.01;
	vec3 w = normalize(eye - look);
	vec3 u = normalize(cross(up, w));
	vec3 v = normalize(cross(w, u));

	u *= (xpos - width/2) * moveSpeed;
	v *= (ypos - height/2) * moveSpeed;
	look += u - v;

	// phi is actually sin(angle)
	float phi = (look[1]-eye[1])/length(look-eye);
	
	// if look angle is out of range, then reverse change
	if( 0.985 < phi || phi < -0.985)
		look += v;
	
	glfwSetCursorPos(window,width/2,height/2);	
}

static void resize_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}


static void init()
{
	srand(time(NULL));
	GLSL::checkVersion();

	// Set background color.
	glClearColor(.12f, .34f, .56f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Initialize mesh.
	cube = make_shared<Shape>();
	cube->loadMesh(RESOURCE_DIR + "cube.obj");
	cube->resize();
	cube->init();

	half_pyramid = make_shared<Shape>();
	half_pyramid->loadMesh(RESOURCE_DIR + "half_pyramid.obj");
	half_pyramid->resize();
	half_pyramid->init();

	sphere = make_shared<Shape>();
	sphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	sphere->resize();
	sphere->init();

	// inittisdjas
	
	// Init shaders

	phung = make_shared<Program>();
	phung->setVerbose(true);
	phung->setShaderNames(RESOURCE_DIR + "phung_vert.glsl", RESOURCE_DIR + "phung_frag.glsl");
	phung->init();
	phung->addUniform("P");
	phung->addUniform("V");
	phung->addUniform("M");
	phung->addAttribute("vertPos");
	phung->addAttribute("vertNor");
	phung->addUniform("MatAmb");
	phung->addUniform("MatDif");
	phung->addUniform("MatSpec");
	phung->addUniform("shine");
	phung->addUniform("lightPos");
	phung->addUniform("shadow");

	boids = new Boids(40);
	boids->update();
}

static void render()
{
	updateEyePosition();
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use the matrix stack for Lab 6
	float aspect = width/(float)height;	

	// Create the matrix stacks
	auto P = make_shared<MatrixStack>();
	mat4 V = glm::lookAt(eye, look, up);;
	auto M = make_shared<MatrixStack>();
	
	// Apply perspective projection.
	P->pushMatrix();
	P->perspective(45.0f, aspect, 0.01f, 100.0f);
	M->loadIdentity();
	
	/* Create light source */
	phung->bind();
		glUniformMatrix4fv(phung->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(phung->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		M->pushMatrix();

			glUniform3f(phung->getUniform("lightPos"), lightPos[0], lightPos[1], lightPos[2]);
			glUniform3f(phung->getUniform("MatAmb"),1,2,2);
			glUniform3f(phung->getUniform("MatDif"),0,0,0);
			glUniform3f(phung->getUniform("MatSpec"), 0,0,0);
			glUniform1f(phung->getUniform("shine"),1);
				  		
			M->translate(vec3(lightPos[0], lightPos[1], lightPos[2]));
			M->scale(vec3(0.1,0.1,0.1));

  			glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
  			sphere->draw(phung);	
		M->popMatrix();
	phung->unbind();
	
	phung->bind();
		glUniformMatrix4fv(phung->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(phung->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glUniform3f(phung->getUniform("lightPos"), lightPos[0], lightPos[1], lightPos[2]);
		glUniform3f(phung->getUniform("MatAmb"),0.5,0.5,0.5);
		glUniform3f(phung->getUniform("MatDif"),0.3,0.3,0.3);
		glUniform3f(phung->getUniform("MatSpec"), 0.3,0.3,0.3);
		glUniform1f(phung->getUniform("shine"),10);
		
		boids->update();
		vector<Boid>::iterator it;	
		for(it = boids->begin() ; it != boids->end() ; ++it)
		{
			M->pushMatrix();
				M->scale(vec3(0.1,0.1,0.1));
				M->translate((*it).position);
	  			glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
	  			sphere->draw(phung);
			M->popMatrix();
		}

	phung->unbind();

	// Pop matrix stacks.
	P->popMatrix();

	vec3 lightMov = normalize(cross(lightPos, vec3(0,1,0)));
	lightMov *= 0.1;
	lightPos += lightMov;
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
   //request the highest possible version of OGL - important for mac
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "YOUR NAME", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	//weird bootstrap of glGetError
   	glGetError();
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
   	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
   	//set the mouse call back
   	glfwSetMouseButtonCallback(window, mouse_callback);
   	//set the window resize call back
   	glfwSetFramebufferSizeCallback(window, resize_callback);
   	//set mouse movement call back
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	// Initialize scene. Note geometry initialized in init now
	init();

	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
