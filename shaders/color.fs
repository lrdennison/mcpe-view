#version 330 core

// Interpolated values from the vertex shaders

in vec3 fragmentColor;

// Output data
out vec3 color;

void main()
{
  // Output color = red 
  // color = vec3(1,0,0);
  color = fragmentColor;

}
