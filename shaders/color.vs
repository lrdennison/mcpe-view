#version 330 core

// Color vertex shader

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

// Values that stay constant for the whole mesh.

uniform mat4 M;
uniform mat4 V;
uniform mat4 MVP;

out vec3 fragmentColor;

void main(){

  gl_Position =  MVP * vertexPosition_modelspace;
    
  // The color of each vertex will be interpolated
  // to produce the color of each fragment

  fragmentColor = vertexColor;
}
