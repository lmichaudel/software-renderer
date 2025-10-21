# Software Renderer

Pure C++ software renderer, supports vertex and fragment shader, and has a depth-buffer.

SDL2 is used to open a window and access raw pixel data, this can be ditched and you can directly write a buffer to a ppm file (RGB for color buffer, Greyscale for depth) with `file << buffer;`  
