#include "gd_shader.h"

LoggerPtr GdShader::logger(Logger::getLogger("GdShader"));

GdShader::GdShader(int width, int height)
{
  LOG4CXX_INFO(logger, "constructor");

  glGenTextures(1, &texture);

  this->width = width;
  this->height = height;
  
  image = gdImageCreateTrueColor(width, height);
}


void GdShader::move_data_to_buffers()
{
  vertices.clear();
  uv.clear();

  float lx, ly, ux, uy;
  lx = x;
  ly = y;
  ux = lx+width;
  uy = ly+height;

  vertices.push_back( glm::vec4( lx, ly, 0.0, 1.0)); uv.push_back(glm::vec2( 0.0, 0.0));
  vertices.push_back( glm::vec4( ux, ly, 0.0, 1.0)); uv.push_back(glm::vec2( 1.0, 0.0));
  vertices.push_back( glm::vec4( lx, uy, 0.0, 1.0)); uv.push_back(glm::vec2( 0.0, 1.0));

  vertices.push_back( glm::vec4( lx, uy, 0.0, 1.0)); uv.push_back(glm::vec2( 0.0, 1.0));
  vertices.push_back( glm::vec4( ux, uy, 0.0, 1.0)); uv.push_back(glm::vec2( 1.0, 1.0));
  vertices.push_back( glm::vec4( ux, ly, 0.0, 1.0)); uv.push_back(glm::vec2( 1.0, 0.0));

  TextureShader::move_data_to_buffers();

  int width = image->sx;
  int height = image->sy;
  char *ndata = new char[width*height*4];
  
  char *p = &ndata[0];
  for( int iy=height-1; iy>=0; iy--) {
    for( int jx=0; jx<width; jx++) {
      int pixel = gdImageGetPixel( image, jx, iy);
      *p++ = (pixel >> 16) & 0xFF;
      *p++ = (pixel >>  8) & 0xFF;
      *p++ = (pixel >>  0) & 0xFF;
      *p++ = 0x80;
    }
  }
		
  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, texture);
  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ndata);

  delete[] ndata;
  
  // ... nice trilinear filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
  glGenerateMipmap(GL_TEXTURE_2D);

}
 
