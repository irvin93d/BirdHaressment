# Bird Haressment
Author: [Duane Irvin](https://github.com/irvin93d)  
Instructor: [Zoë Wood](http://users.csc.calpoly.edu/~zwood/teaching/csc471/index16.html)  
Course: CPE 471  
University: California Polytechnic State University, San Luis Obispo, USA  

## Description
Bird Haressment is a graphical implementation of boids, using OpenGL and C++. It simulates the moving patterns of a flock of birds based on the three rules: Seperation, Cohesion and Alignment. A bullet can be shot towards the birds, wheras they will avoid the bullet and break their flying pattern. 

## Getting Started

### Compile and run

Clone the repository and cd into it:
```console
  git clone https://github.com/irvin93d/BirdHaressment.git
  cd BirdHaressment
```
Create and cd into build folder:
```console
  mkdir build
  cd build
```
Compile project:
```console
  cmake ..
  make -j4
```
Run program:
```
  ./program ../resources
```
Note that GLM, GLFW and and GLEW variables must be set up for program to run.

### Controls
- W/S - Move forward/backwar 
- A/D - Strafe left/right
- E/Q - Fly up/down
- LMouse - Shoot
- P - Stop/Run birds
- ESC - Quit

## Features Implemented

### Required
- [x] Birds shall be built on hierarchical modeling.
- [x] Birds shall move within their own model, such as having wing movement.
- [x] Birds shall use boids to create their moving patterns in a somewhat realistic way.
- [x] Birds shall fly in their own pointing direction, i.e. birds shall not strafe or fly backwards.
- [x] There shall be some way to interrupt the birds’ flying path.
- [x] A sun shall represent the light source.
- [x] Acceleration properties shall be implemented for the moving birds to give them a more realistic speed differentiation.

### Recommended
- [ ] Environment shall contain hierarchical modelled trees in different sizes.
- [ ] Shadows shall exist from all objects, correctly adjusted to sun.
- [x] The sun shall move while time passes by.
- [x] Sky shall be gradient in relationship to the sky.
- [ ] Trees shall be growing using fractals as time passes by.

### Optional
- [ ] Terrain generated by fractals should exist, preferably different on every program start.
- [ ] Implement markov chains to give birds and/or different probabilities of their behavior, such as new moving direction for birds and different ways of growing for trees.
- [ ] Birds shall “eat” other birds when colliding, causes them to grow. New birds shall appear to keep the number somewhat consistent.
- [ ] Blood shall be spilled from cannibalism.

## Resources 
- Boids example in 2-dimensions, a lot of this source code have been rewritten to C++: [Flocking - Processing.org](https://www.processing.org/examples/flocking.html)

## Preview 

### Screenshots

![Screenshot 1](./web/res/screen1.png)

![Screenshot 2](./web/res/screen2.png)

### Demo video
[![Bird Haressment Demo at Youtube](https://img.youtube.com/vi/fkjIEVBFNgQ/0.jpg)](https://youtu.be/fkjIEVBFNgQ)



