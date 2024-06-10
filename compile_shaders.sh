#!/bin/bash

glslc -fshader-stage=vertex -c shaders/vert.glsl -o shaders/vert.spv
glslc -fshader-stage=fragment -c shaders/frag.glsl -o shaders/frag.spv
