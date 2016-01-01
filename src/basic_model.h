#ifndef BASIC_MODEL_H
#define BASIC_MODEL_H

#include "common.h"
#include "vertex_stream.h"

class BasicModel {
 public:

  virtual ~BasicModel();
  virtual void render( VertexStream &vs, glm::mat4 &xform);
};


#endif
