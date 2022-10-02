# Milestone 1: 3d cube presented with navigation [Done]

## Key results

- a 3d cube is rendered
- camera motion is controlled by the mouse, and the 4 standard movement keys

## Value

We bootstrapped the working environement and built an understanding of the core frameworks and libraries.

## Artifacts

- file: cube.cc
  - git repo: https://github.com/NandorKovacs/4dvisualizer
  - git commit: 985ab8f05dfe445aff72ad5c9819c02d87ca8a61


# Milestone 2: 4d cube presented with navigation [In-progress]

## Key results

A c++ program is delivered, which 

- presents a 3d projection of a 4d cube
- the user can adjust the rotation angle of the 4d cube via keys
- the user can move the projection plane via keys
- the user move the camera via keys
- values of the parameters are displayed as a hud
  - this is useful to allow storing of reproducible screenshots

## Value

We have a skeleton of the core of the code with end to end functionality.

# Milestone 3: visibility effects

## Key Results

Object shading and lighting will be implemented.

## Value

The program allows us to experiment with parameters, form a visual idea 
about various projection shapes of the 4d cube and use it as an experiment and development platform 
for subsequent milestones.

With object shading and lighting the rendered picture will be more easily understood by the user. 

# Milestone 4: World description

## Key results

- File format is defined to describe 4d objects
- parser is written 

## Value

We are ready to assemble the minimum project.

# Milestone 5: Full world rendering

## Key results

Program that can render a World described a config file, allows moving the projection plane as well as the camera in the 3d world.
Provides feedback of projection and camera position in a HUD.

## Value

This is a minimum viable solution of the original project description.