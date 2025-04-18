# Pathfinding

A sandbox based on a simple OpenGL game engine, showcasing the implementation of pathfinding algorithms (currently only A* pathfinding, with potential additions in the future). It finds optimal paths on a grid-based map while considering obstacles, movement costs, and search heuristics.

## Showcase
<p align="center">
  <img src="https://github.com/mingthang/pathfinding/blob/master/astar-pathfinding/res/astar.gif" width="50%" />
</p>

## Controls
- **Left Mouse Button**: Add an obstacle at the mouse position.
- **Right Mouse Button**: Remove an obstacle at the mouse position.
- **1 Key**: Set the start point at the mouse position.
- **2 Key**: Set the end point (target) at the mouse position.
- **N Key**: Clear the map.
- **Q Key**: Toggle slow mode.
- **Spacebar (Pressed/Hold)**: Begin pathfinding search.
- **W Key**: Smooth the found path.
- **E Key**: Smooth the found path (slow mode version).
- **` Key**: Toggle debug text mode.
- **G Key**: Toggle windowed/fullscreen mode.

## Third Party Libraries Used
### GLFW
- **Version**: 3.4
- **Website**: [https://www.glfw.org/](https://www.glfw.org/)
- **Description**: GLFW library for creating windows, contexts, and handling input.

### GLAD
- **Version**: 0.1.36
- **Website**: [https://glad.dav1d.de/](https://glad.dav1d.de/)
- **Description**: OpenGL loader for managing OpenGL function pointers.

### stb_image.h
- **Version**: 2.30
- **Repository**: [https://github.com/nothings/stb/blob/master/stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
- **Description**: A single-header library for image loading in OpenGL applications.

### GLM
- **Version**: 1.0.1
- **Repository**: [https://github.com/g-truc/glm](https://github.com/g-truc/glm)
- **Description**: OpenGL Mathematics header-only library.

### FMOD
- **Version**: 2.0.3
- **Link**: [fmod.com](https://www.fmod.com/)
- **Description**: For integration of the FMOD run-time with custom engines.
