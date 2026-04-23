# LearnOpenGL
Learning OpenGL with the information of the https://learnopengl.com/Introduction steps

## Dependencies

This project relies on several libraries to handle window creation, math, UI, profiling, and OpenGL function loading. The build system is configured to fetch most of these automatically, while others must be installed on your system.

### Automatically Fetched (via CMake)
You do not need to install these manually; CMake will download and build them automatically when you configure the project:
* **[GLFW](https://www.glfw.org/)** (v3.3.8): Handles operating system-level tasks like creating the application window, managing the OpenGL context, and processing input (keyboard/mouse).
* **[GLM](https://github.com/g-truc/glm)** (v0.9.9.8): OpenGL Mathematics library used for vector and matrix operations.
* **[Dear ImGui](https://github.com/ocornut/imgui)** (v1.90.1): Bloat-free graphical user interface library for C++. Used for easily creating debug windows, tools, and UI overlays rendered directly on top of the OpenGL canvas.
* **[Tracy](https://github.com/wolfpld/tracy)** (v0.11.1): A real-time, nanosecond resolution, remote telemetry, hybrid frame and sampling profiler for games and other applications. 
  * **Tracy Client**: Keeps in the background, collects all the timing data from your macros and opens a port to broadcast that data.
    * Use the `FrameMark;` macro at the end of your render loop to mark frame boundaries and calculate FPS.
    * Use the `ZoneScoped;` macro at the beginning of any function to accurately measure its execution time in the Tracy server GUI.
  * **Tracy Profiler** or **Tracy Server**: It connects to your running application, downloads the telemetry data over your local network, and draws graphs. **Note that you must have the standalone Tracy Profiler server running to view the profiling data.**

### System Requirements
These dependencies must be installed on your operating system for the project to build successfully:
* **OpenGL**: The core graphics API. On Linux, this is typically provided by your graphics drivers (e.g., `libGL.so`).
* **[GLEW](http://glew.sourceforge.net/)** (OpenGL Extension Wrangler): Loads modern OpenGL functions from your graphics driver at runtime.
  * *Ubuntu/Debian installation:* `sudo apt-get install libglew-dev`

# LearnOpenGL 1: Getting Started
First there are the changes to getting started with OpenGL features, objects and so on.

# LearnOpenGL 2: Lighting

# LearnOpenGL 3: Model Loading

# LearnOpenGL 4: Advanced OpenGL

# LearnOpenGL 5: Advanced Lighting

# LearnOpenGL 6: PBR

# LearnOpenGL 7: In Practice

# LearnOpenGL 8: Guest Articles
