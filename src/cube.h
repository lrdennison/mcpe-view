#ifndef CUBE_H
#define CUBE_H

#include "basic_model.h"
#include "texture_atlas.h"

class Cube : public BasicModel {
 public:

  int cnt;
  int face_cnt;
  
  GLfloat x[36], y[36], z[36];
  GLfloat u[36], v[36];

  glm::vec4 xyz[36];
  glm::vec2 uv[36];
  
  Cube();
  void render( VertexStream &vs, glm::mat4 &xform, UVS &uvs);

  void mk_face( GLfloat d0[], GLfloat d1[], GLfloat d2[], GLfloat d2v);

  
};

#endif
