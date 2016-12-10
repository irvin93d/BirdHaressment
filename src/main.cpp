/* Lab 6 base code - transforms using matrix stack built on glm 
	CPE 471 Cal Poly Z. Wood + S. Sueda
*/
#include <iostream>
#define GLEW_STATIC
#define WORLD_SIZE 30

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Flock.h"
#include "Texture.h"
#include "Bullets.h"

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

// Shaders
shared_ptr<Program> phung;
shared_ptr<Program> atmosphere;
shared_ptr<Program> grndShader;

// Light and camera
vec3 eye = vec3(0,2,-2);
vec3 look = normalize(vec3(0,2,0) - eye) + eye; // Make sure eye to look point has length 1
vec3 up = vec3(0,1,0);
vec3 eyeMov = vec3(0,0,0); // set flags for smooth camera movement
vec3 lightPos = vec3(300,200,0);

// Objects
shared_ptr<Shape> half_pyramid;
shared_ptr<Shape> sphere;
shared_ptr<Shape> cube;

// Object handlers
Flock* flock;
Bullets* bullets;

// Ground stuff
Texture grass;
int g_GiboLen;
GLuint GrndBuffObj, GrndNorBuffObj, GrndTexBuffObj, GIndxBuffObj;

// Some variables
bool flockRunning = true;

float randFloat(float min, float max) {
	float range = max - min;
	float num = range * rand() / RAND_MAX;
	return (num + min);
}

void addRandBoid(){
	flock->addBoid(
			Boid(
					vec3(randFloat(-WORLD_SIZE,WORLD_SIZE),randFloat(4,WORLD_SIZE),randFloat(-WORLD_SIZE,WORLD_SIZE)),
					vec3(randFloat(-0.1,0.1),randFloat(-0.1,0.1),randFloat(-0.1,0.1)),
					vec3(randFloat(0,1),randFloat(0,1),randFloat(0,1)),
					vec3(randFloat(0,1),randFloat(0,1),randFloat(0,1)),
					vec3(randFloat(0,1),randFloat(0,1),randFloat(0,1))
				)
		);
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
			eyeMov.z--;
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
	if(key == GLFW_KEY_E) {
		if(action == GLFW_PRESS)
			eyeMov.y++;
		else if(action == GLFW_RELEASE)
			eyeMov.y--;
	}
	if(key == GLFW_KEY_Q) {
		if(action == GLFW_PRESS)
			eyeMov.y--;
		else if(action == GLFW_RELEASE)
			eyeMov.y++;
	}
	if(key == GLFW_KEY_KP_ADD){
		addRandBoid();
	}
	if(key == GLFW_KEY_KP_SUBTRACT){
		flock->removeBoid();
	}
	if(key == GLFW_KEY_P && action == GLFW_PRESS){
		flockRunning = !flockRunning;
	}
}

static void updateCamera()
{
	float moveSpeed = 0.3;
	vec3 w = normalize(eye - look);
	vec3 u = normalize(cross(up, w));

	// Dolly
	w *= moveSpeed*eyeMov.z;
	eye -= vec3(w[0], 0, w[2]);
	look -= vec3(w[0], 0, w[2]);
	
	// strafe
	u *= moveSpeed*eyeMov.x;
	eye += u;
	look += u;

	// flyyyy
	eye += vec3(0,moveSpeed * eyeMov.y,0);
	look += vec3(0,moveSpeed * eyeMov.y,0);
}

static void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
	double posX, posY;
	if (action == GLFW_PRESS) {
		glfwGetCursorPos(window, &posX, &posY);
		bullets->addBullet(eye, normalize(look - eye));
	}
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	float moveSpeed = 0.002;
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
	
	// Reset pointer to middle
	glfwSetCursorPos(window,width/2,height/2);	
}

static void resize_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

/* code to define the ground plane */
static void initGeom() {

	float g_groundSize = WORLD_SIZE;
	float g_groundY = 0;

	// A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
	float GrndPos[] = {
		-g_groundSize, g_groundY, -g_groundSize,
		-g_groundSize, g_groundY,  g_groundSize,
		g_groundSize, g_groundY,  g_groundSize,
		g_groundSize, g_groundY, -g_groundSize
	};

	float GrndNorm[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	static GLfloat GrndTex[] = {
		0, 0, // back
		0, 6,
		6, 6,
		6, 0
	};

	unsigned short idx[] = {0, 1, 2, 0, 2, 3};

	GLuint VertexArrayID;
	//generate the VAO
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	g_GiboLen = 6;
	glGenBuffers(1, &GrndBuffObj);
	glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

	glGenBuffers(1, &GrndNorBuffObj);
	glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

	glGenBuffers(1, &GrndTexBuffObj);
	glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

	glGenBuffers(1, &GIndxBuffObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
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

	// Init textures
	grass.setFilename(RESOURCE_DIR + "grass.bmp");
	grass.setUnit(2);
	grass.setName("Texture");
	grass.init();

	// Init mesh objects
	half_pyramid = make_shared<Shape>();
	half_pyramid->loadMesh(RESOURCE_DIR + "half_pyramid.obj");
	half_pyramid->resize();
	half_pyramid->init();

	sphere = make_shared<Shape>();
	sphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	sphere->resize();
	sphere->init();

	cube = make_shared<Shape>();
	cube->loadMesh(RESOURCE_DIR + "cube.obj");
	cube->resize();
	cube->init();

	// Init object handlers
	flock = new Flock();
	bullets = new Bullets();
	flock->bullets = bullets;

	for(int i = 0 ; i < 100 ; i++){
		addRandBoid();
	}

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

	atmosphere = make_shared<Program>();
	atmosphere->setVerbose(true);
	atmosphere->setShaderNames(RESOURCE_DIR + "atmo_vert.glsl", RESOURCE_DIR + "atmo_frag.glsl");
	atmosphere->init();
	atmosphere->addUniform("P");
	atmosphere->addUniform("V");
	atmosphere->addUniform("M");
	atmosphere->addAttribute("vertPos");
	atmosphere->addAttribute("vertNor");
	atmosphere->addUniform("lightPos");

	grndShader = make_shared<Program>();
	grndShader->setVerbose(true);
	grndShader->setShaderNames(RESOURCE_DIR + "ground_vert.glsl", RESOURCE_DIR + "ground_frag.glsl");
	grndShader->init();
	grndShader->addUniform("P");
	grndShader->addUniform("V");
	grndShader->addUniform("M");
	grndShader->addUniform("Texture");
	grndShader->addAttribute("vertPos");
	grndShader->addAttribute("vertNor");
	grndShader->addAttribute("vertTex");
	grndShader->addUniform("lightPos");
	grndShader->addTexture(&grass);
}

static void render()
{
	static float t = 0 ; // timer for wing and mouth movement
	static float td = -0.2; // timerdifference
	t += td;
	if(t > 0 || t < -1)
		td *= -1; // wings and mouth goes back and forth

	// update positions
	updateCamera(); 
	bullets->run(); // update bullets
	if(flockRunning)
		flock->run(); // update flocks
	vec3 lightMov = normalize(cross(lightPos, vec3(0,1,0)));
	lightMov *= 0.1;
	lightPos += lightMov;

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
	mat4 V = glm::lookAt(eye, look, up);
	auto M = make_shared<MatrixStack>();
	
	// Apply perspective projection.
	P->pushMatrix();
	P->perspective(45.0f, aspect, 0.01f, 100.0f);
	M->loadIdentity();
	
	atmosphere->bind();
		glUniformMatrix4fv(atmosphere->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(atmosphere->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glUniform3f(atmosphere->getUniform("lightPos"), lightPos[0], lightPos[1], lightPos[2]);
		M->pushMatrix();
			M->translate(eye);
			M->scale(vec3(100,100,100));
  			glUniformMatrix4fv(atmosphere->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
  			sphere->draw(atmosphere);	
		M->popMatrix();
	atmosphere->unbind();

	phung->bind();
		// draw light source 
		glUniformMatrix4fv(phung->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(phung->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glUniform3f(phung->getUniform("lightPos"), lightPos[0], lightPos[1], lightPos[2]);
		glUniform3f(phung->getUniform("MatAmb"),3,3,1.7);
		glUniform3f(phung->getUniform("MatDif"),0,0,0);
		glUniform3f(phung->getUniform("MatSpec"), 0,0,0);
		glUniform1f(phung->getUniform("shine"),1);
		M->pushMatrix();
			vec3 drawnLightPos = normalize(lightPos - eye);
			drawnLightPos *= 30;
			drawnLightPos += eye;

			M->translate(drawnLightPos);
			M->scale(vec3(0.7,0.7,0.7));
  			glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
  			sphere->draw(phung);	
		M->popMatrix();
		
		// draw bullets	
		glUniform3f(phung->getUniform("MatAmb"),0.2,0.2,0.2);
		glUniform3f(phung->getUniform("MatDif"),0.2,0.2,0.2);
		glUniform3f(phung->getUniform("MatSpec"), 0.3,0.3,0.3);
		glUniform1f(phung->getUniform("shine"),10);
		for(vector<Bullet>::iterator it = bullets->bullets.begin() ; it != bullets->bullets.end() ; ++it)
		{
			M->pushMatrix();
				// World position
				M->translate(it->pos);
				// Bullet size
				M->scale(vec3(0.1,0.1,0.1));
				glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
				sphere->draw(phung);
			M->popMatrix();
		}

		// draw birds
		for(vector<Boid>::iterator it = flock->boids.begin() ; it != flock->boids.end() ; ++it)
		{
			vec3 amb = it->ambient;
			vec3 diff = it->diffuse;
			vec3 spec = it->specular;
			glUniform3f(phung->getUniform("MatAmb"),amb.x, amb.y, amb.z);
			glUniform3f(phung->getUniform("MatDif"),diff.x, diff.y, diff.z);
			glUniform3f(phung->getUniform("MatSpec"),spec.x, spec.y, spec.z);
		
			M->pushMatrix();
				// position world
				M->translate(it->position); 
				// total size
				M->scale(vec3(0.1,0.1,0.1));
				// rotation
				vec3 velocity = normalize(it->velocity); 
				if(velocity.z > 0 )
					M->rotate(asin(velocity.x),vec3(0,1,0));
				else
					M->rotate(M_PI-asin(velocity.x),vec3(0,1,0));
				M->rotate(-asin(velocity.y),vec3(1,0,0));
				
				// actual body transformation and drawing comes below
				M->pushMatrix();
					// head and mouth below
					M->translate(vec3(0,-0.2,3));
					M->scale(vec3(0.5,0.5,0.5));
					M->rotate(0.2,vec3(1,0,0));
					M->pushMatrix();
						// mouth 
						M->translate(vec3(0,0,1));
						M->scale(vec3(0.5,0.5,0.5));
						M->pushMatrix();
							// bottom of mouth
							M->rotate(M_PI, vec3(0,0,1));
							M->rotate(t/2, vec3(1,0,0));
							M->scale(vec3(1,0.5,1));
							M->translate(vec3(0,1,1));
							glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
							half_pyramid->draw(phung);
						M->popMatrix();
						// top of mouth
						M->rotate(t/2, vec3(1,0,0));
						M->scale(vec3(1,0.5,1));
						M->translate(vec3(0,1,1));
						glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
						half_pyramid->draw(phung);
					M->popMatrix();
					// skull
					M->translate(vec3(0,0,-0.9));
					M->scale(vec3(1,1.2,2));
					glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
					sphere->draw(phung);
				M->popMatrix();

				// Left Wing
				M->pushMatrix();
					M->translate(vec3(0.65,0,0.3));
					M->rotate(t+0.5,vec3(0,0,1));
					M->pushMatrix();
						M->translate(vec3(1.35,0,0));
						M->rotate(0.5, vec3(0,1,-1));
						M->scale(vec3(0.8,0.1,0.6));
						M->translate(vec3(1,0,0));
						glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
						cube->draw(phung);
					M->popMatrix();
					M->scale(vec3(0.8,0.1,0.6));
					M->translate(vec3(1,0,0));
					glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
					cube->draw(phung);
				M->popMatrix();

				// Right Wing
				M->pushMatrix();
					M->translate(vec3(-0.65,0,.3));
					M->rotate(t+0.5,vec3(0,0,-1));
					M->pushMatrix();
						M->translate(vec3(-1.35,0,0));
						M->rotate(0.5, vec3(0,-1,1));
						M->scale(vec3(0.8,0.1,0.6));
						M->translate(vec3(-1,0,0));
						glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
						cube->draw(phung);
					M->popMatrix();
					M->scale(vec3(0.8,0.1,0.6));
					M->translate(vec3(-1,0,0));
					glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
					cube->draw(phung);
				M->popMatrix();

				// body below
				M->scale(vec3(0.7,0.7,2.5));
				glUniformMatrix4fv(phung->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
				sphere->draw(phung);
			M->popMatrix();
		}

	phung->unbind();

	// Draw ground, nada más
	grndShader->bind();
		glUniformMatrix4fv(grndShader->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(grndShader->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glUniformMatrix4fv(grndShader->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		glUniform3f(grndShader->getUniform("lightPos"), lightPos[0], lightPos[1], lightPos[2]);
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// draw!
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
		glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	grndShader->unbind();

	// Pop matrix stacks.
	P->popMatrix();
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
	window = glfwCreateWindow(640, 480, "Bird Haressment", NULL, NULL);
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
	initGeom();

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
