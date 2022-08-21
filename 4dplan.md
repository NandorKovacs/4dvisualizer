# Milestone 1: 3d cube presented with navigation

# Milestone 2: 4d cube presented with navigation

## Key results

A c++ program is delivered, which 

- presents a 3d projection of a 4d cube
- the user can adjust the rotation angle of the 4d cube via keys
- the user can move the projection plane via keys
- the user move the camera via keys
- values of the parameters are displayed as a hud
  - this is usefull to allow storing of reproducible screenshots

## Value

The program allows us to experiment with parameters, form a visual idea 
about various projection shapes of the 4d cube and use it as an experiment and development platform 
for subsequent milestones.

## Implementation

an object, called 4d_cube

has a method, which takes a reference to a vertex array, and an int which specifies the coordinate of the fourth dimension
this method places all the vertecies into the array

it has a method which returns the model matrix

it has the location of the cube, in the four dimensional space
(it has the rotation of the cube, along all four axes)

every rendered cube will need an instance of this class. There will be a vector with 4d_cubes. 
During initiation, the model matrices get stored in an array. If the 4th coordinate of the camera changes, vertices will be refreshed.
