
# Bird Haressment
Author: [Duane Irvin](https://github.com/irvin93d)  
Instructor: [Zoë Wood](http://users.csc.calpoly.edu/~zwood/teaching/csc471/index16.html)  
Course: CPE 471  
University: California Polytechnic State University, San Luis Obispo, USA  

## Description
Bird Haressment is a graphical implementation of boids, using OpenGL and C++. It simulates the moving patterns of a flock of birds based on the three rules: Seperation, Cohesion and Alignment. A bullet can be shot towards the birds, wheras they will avoid the bullet and break their flying pattern. 

## Compile and run

Clone the repository and cd into it:
```console
  git clone https://github.com/irvin93d/CPE471-FinalProject.git
  cd CPE471-FinalProject
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

## Screenśhots

TODO:

![Screenshot 1](http://placekitten.com/400/300)

![Screenshot 2](http://placekitten.com/400/300)
