#!/bin/zsh

/usr/local/bin/glslc shaders/triangle.vert -o bin/vert.spv
/usr/local/bin/glslc shaders/triangle.frag -o bin/frag.spv