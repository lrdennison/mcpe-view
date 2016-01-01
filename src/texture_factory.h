#ifndef TEXTURE_FACTORY_H
#define TEXTURE_FACTORY_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "common.h"

class TextureFactory : public Singleton<TextureFactory>
{
public:

  static GLuint get(std::string name)
  {
    return Inst()._get( name);
  }
  
  
protected:
  std::map<std::string, GLuint> m;

  gdImagePtr image;
  char *ndata;

  std::string readEntireFile(std::string filename)
  {
    FILE *in;
    struct stat stat_buf;

    in = fopen(filename.c_str(), "rb");
    if (!in) {
      /* Error */
    }
    if (fstat(fileno(in), &stat_buf) != 0) {
      /* Error */
    }
    /* Read the entire thing into a buffer
       that we allocate */
    char *buffer = (char *)malloc(stat_buf.st_size);
    if (!buffer) {
      /* Error */
    }
    if (fread(buffer, 1, stat_buf.st_size, in)
	!= stat_buf.st_size)
      {
	/* Error */
      }

    std::string s( buffer, stat_buf.st_size);
    free(buffer);
    return s;
  }
    

  void loadBmp(std::string filename)
  {
    std::string s;
    s = readEntireFile( filename);

    image = gdImageCreateFromPngPtr(s.length(), (char *)s.c_str());
  }

  void make_texture_data()
  {
    int width = image->sx;
    int height = image->sy;
    ndata = new char[width*height*4];

    char *p = &ndata[0];
    for( int iy=height-1; iy>=0; iy--) {
      for( int jx=0; jx<width; jx++) {
	int pixel = gdImageGetPixel( image, jx, iy);
	*p++ = (pixel >> 16) & 0xFF;
	*p++ = (pixel >>  8) & 0xFF;
	*p++ = (pixel >>  0) & 0xFF;

	// GD only uses 7 bits for alpha
	// Scale to the 0-255 range
	// In GD, completely transparent is 127
	// so we make sure that maps to zero
	
	int i = (pixel >> 24) & 0x7F;
	int v = 255 - 2*i;
	if( i ==127)
	  v = 0;
	*p++ = v;
      }
    }
  }


  int load(std::string path)
  {
    GLuint texture;
    
    loadBmp(path);
    int width = image->sx;
    int height = image->sy;

    make_texture_data();
    
    glGenTextures(1, &texture);
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

    return texture;
  }
  

  GLuint _get(std::string name)
  {
    GLuint texture;
    
    if( m.find( name) == m.end()) {
      if( name == "terrain") {
	// texture = loadBMP_custom("/home/larry/mcpe-view/textures/terrain-atlas.bmp");
	texture = load("/home/larry/mcpe-view/textures/terrain-atlas.png");
	m[name] = texture;
      }
    }

    return m[name];
  }
};


#endif
