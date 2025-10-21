# Software Renderer

A pure, header-only, C++ software renderer, supports vertex and fragment shaders, and has a depth-buffer.

###

<p align="center">
  <img width="60%" src="https://github.com/user-attachments/assets/733a8950-d801-4ba5-bfaf-f3722a68a630" />
</p>


The [main.cpp](main.cpp) file contains an example demo : a cube composed of several triangles is inputed : it is transformed using classic matrix transformations in the vertex shader, then each face is colored using barycentric coordinates in the fragment shader. The depth before automatically performs the depth test (see [this if statement](gfxRenderer.hpp#L62)).

SDL2 is used to open a window and access raw pixel data, this can be ditched and you can directly write a buffer to a ppm file (RGB for color buffer, Greyscale for depth) with `file << buffer;`. The library is expected to be installed on your system and is retrieved with pkg-config (see [Makefile](Makefile))

