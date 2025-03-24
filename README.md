**OpenGL Scene Rendering Project**


This project is an OpenGL-based graphics application that renders a 3D scene consisting of various objects, including spheres and cubes. 
The rendering pipeline follows a programmable approach, utilizing custom shaders for vertex and fragment processing. The project implements key graphical techniques 
such as perspective projection, Phong shading, fog effects, day/night cycles, and light attenuation.

**Camera Views**
There are three camera modes that can be switched by pressing 1, 2 and 3 on keyboard.

**Lighting System**
There is a spotlight attached to the moving cube that can be modified using arrows.

**Fog Effect**: Smoothly transitions between different visibility levels, should be visible over time (since it uses time for modification).

**Day/Night Cycle**: Gradual changes in lighting conditions to simulate different times of the day (also should be visible over time).

**Specular Reflection Adjustment**: Ability to switch between Phong and Blinn-Phong shading models (by pressing B key).

**Implementation Details**
OpenGL for rendering.
GLFW for window and input management.
GLAD for OpenGL function loading.
GLM for mathematical operations (vector and matrix transformations).
STB Image for texture loading.
