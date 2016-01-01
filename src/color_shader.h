#ifndef COLOR_SHADER_H
#define COLOR_SHADER_H

#include "common.h"
#include "camera.h"
#include "shader_program.h"

class ColorShader {
public:
  std::vector<glm::vec4> vertices;
  std::vector<glm::vec3> colors;
  
  ColorShader();
  void move_data_to_buffers();
  void draw();

private:
  ShaderProgram *program;

  GLuint vertexbuffer;
  GLuint colorbuffer;

  GLuint mvp_id;

};

#endif
