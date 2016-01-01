#ifndef BLOCK_H
#define BLOCK_H

#include "common.h"
#include "cube.h"


class Block {
 public:

  static Cube *cube;
  glm::mat4 xform;

  unsigned int id; // from 0 to 255
  unsigned int data; // from 0 to 15
  
  Block();
  
  void render( VertexStream &vs);
};

#endif
