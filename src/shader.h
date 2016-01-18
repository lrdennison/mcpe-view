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
  std::vector<glm::vec2> tile;	   // shader location 4

  static const int num_pri = 8;
  std::vector<int> priority;	// Currently supporting 0 thru 7 with 0 being the highest
  int pri_cnts[num_pri];
  
  Shader();
  void use_program(std::string name);
  
  void use_color();
  void use_normal();
  void use_uv();
  void use_tile();
  void use_texture();

  template<class T>
  void move_to_buffer(GLuint buffer, std::vector<T> &v);

  void move_data_to_buffers();
  void draw(int priority=0);

protected:
  ShaderProgram *program;

  // Flags
  bool _color;
  bool _normal;
  bool _uv;
  bool _tile;
  bool _texture;
  
  unsigned int num_vertices;

  GLuint vertexbuffer;
  GLuint colorbuffer;
  GLuint uvbuffer;
  GLuint normalbuffer;
  GLuint tilebuffer;


  GLint mvp_uloc;
  GLint m_uloc;
  GLint v_uloc;
  GLint texture_uloc;

  GLuint texture;

};
