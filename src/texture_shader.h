#ifndef TEXTURE_SHADER_H
#define TEXTURE_SHADER_H

#include "common.h"
#include "camera.h"
#include "shader_program.h"

class TextureShader {
public:
  std::vector<glm::vec4> vertices;
  std::vector<glm::vec2> uv;
  
  TextureShader();
  void move_data_to_buffers();
  void draw();

protected:
  ShaderProgram *program;

  GLuint vertexbuffer;
  GLuint uvbuffer;
  unsigned int num_vertices;

  GLuint mvp_uloc;
  GLuint texture_uloc;

  GLuint texture;
};

#endif
