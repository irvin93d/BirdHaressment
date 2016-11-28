Programming Assignment 4 - Bunnies in the Forrest :D
Author: Duane Irvin

Description
The programming gives a first person view in a randomly generated forrest filled with bunnies. A light source rotates around the forrest, giving light contrasts and shadows to the environment.

Controls
Use WASD to move around, and mouse to look around. Use Q or E to fly away!

Some design comments
- When looking around, a vector perpendicular to looking direction is calculated. The vector is used to compute the next lookAt-point (simpler than using angles and stuff). #vectors4life<3
- While initiating the environment, a temporary 2D array is used to make sure two object won't be created inside each other.
- 4th value in bunny/tree position buffer decides rotation angle/size of bunny/tree.
- The shadow projection uses laws of triangles, while droping the y value of the vertex. Makes a nice projection on the ground. Note that it only works on flat ground, and the light still goes through objects. A weird situation would probably occur if several light sources are used.
