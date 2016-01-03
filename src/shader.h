#include "common.h"
#include "shader_program.h"

// A shader is shader program + user data
// This is in the "struct of arrays" style, nice
// for streaming processing

class Shader {
 public:

  // We have arrays for everything that might be needed

  std::vector<glm::vec4> vertices; // shader location 0
  std::vector<glm::vec3> color;	   // shader location 1
  std::vector<glm::vec2> uv;	   // shader location 2
  std::vector<glm::vec3> normal;   // shader location 3

  Shader();
  void use_program(std::string name);
  
  void use_color();
  void use_normal();
  void use_uv();
  void use_texture();

  void move_data_to_buffers();
  void draw();

protected:
  ShaderProgram *program;

  // Flags
  bool _color;
  bool _normal;
  bool _uv;
  bool _texture;
  
  unsigned int num_vertices;

  GLuint vertexbuffer;
  GLuint colorbuffer;
  GLuint uvbuffer;
  GLuint normalbuffer;


  GLint mvp_uloc;
  GLint m_uloc;
  GLint v_uloc;
  GLint texture_uloc;

  GLuint texture;

};
