#ifndef VERTEX_STREAM_H
#define VERTEX_STREAM_H

// Include standard headers

#include <vector>
#include <string>
#include <exception>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "color_shader.h"
#include "texture_shader.h"
#include "block_shader.h"

using namespace glm;

class VertexStream {
 public:
  class ColorShader *colorShader;
  class BlockShader *textureShader;
};

#endif
