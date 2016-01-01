#ifndef GDSHADER_SHADER_H
#define GDSHADER_SHADER_H

#include "texture_shader.h"

class GdShader : public TextureShader {
public:
  gdImagePtr  image;		// a GD image object
  int width;
  int height;
  float x, y;		// Where to draw it
  
  GdShader(int width, int height);
  void move_data_to_buffers();

protected:
  static LoggerPtr logger;

};

#endif

