#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "texture_factory.h"


std::string TextureFactory::readEntireFile(std::string filename)
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
    

void TextureFactory::loadBmp(std::string filename)
{
  std::string s;
  s = readEntireFile( filename);

  image = gdImageCreateFromPngPtr(s.length(), (char *)s.c_str());
}

  
char *TextureFactory::make_texture_data(gdImagePtr image)
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

  return ndata;
}



char *TextureFactory::make_texture_array_data(gdImagePtr image)
{
  int width = image->sx;
  int height = image->sy;
  ndata = new char[width*height*4];

  int tile_width = 64;
  int tile_height = 64;

  int num_tiles_x = width / tile_width;
  int num_tiles_y = height / tile_height;

  char *p = &ndata[0];

  for( int ty=0; ty<num_tiles_y; ty++) {
    for( int tx=0; tx<num_tiles_x; tx++) {
      
      for( int iy=tile_height-1; iy>=0; iy--) {
	for( int ix=0; ix<tile_width; ix++) {

	  int px = tx*tile_width + ix;
	  int py = ty*tile_height + iy;
	      
	  int pixel = gdImageGetPixel( image, px, py);
	  // pixel = 0x00808080;
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
  }

  return ndata;
}



gdImagePtr TextureFactory::make_scaled_image(gdImagePtr master_image, int master_tile_size, int divide_by)
{
  gdImagePtr scaled_image;
  int width = image->sx;
  int height = image->sy;
  int num_tiles_x = width / master_tile_size;
  int num_tiles_y = height / master_tile_size;
  int new_tile_size = master_tile_size / divide_by;

  scaled_image = gdImageCreateTrueColor(width / divide_by, height / divide_by);

  for(int x=0; x<num_tiles_x; x++) {
    for(int y=0; y<num_tiles_y; y++) {
      int sx = x*master_tile_size;
      int sy = y*master_tile_size;
      int dx = x*new_tile_size;
      int dy = y*new_tile_size;
      gdImageCopyResampled(scaled_image, master_image,
			   dx, dy,
			   sx, sy,
			   new_tile_size, new_tile_size,
			   master_tile_size, master_tile_size
			   );
    }
  }

  char fn[256];
  sprintf( fn, "foo%d.png", new_tile_size);
  FILE *fp = fopen( fn, "w");
  gdImagePng( scaled_image, fp);
  fclose( fp);
  return scaled_image;
}

  

int TextureFactory::load(std::string path)
{
  GLuint texture;
    
  loadBmp(path);
  int width = image->sx;
  int height = image->sy;
    
  int mipLevelCount = 1;
  int tile_width = 64;
  int tile_height = 64;

  int num_tiles_x = width / tile_width;
  int num_tiles_y = height / tile_height;
  int num_tiles = num_tiles_x * num_tiles_y;

  make_texture_array_data(image);
    
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

  // Texture array way

  glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, tile_width, tile_height, num_tiles);
  //Upload pixel data.
  //The first 0 refers to the mipmap level (level 0, since there's only 1)
  //The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
  //The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
  //Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, tile_width, tile_height, num_tiles, GL_RGBA, GL_UNSIGNED_BYTE, ndata);

  //Always set reasonable texture parameters
  glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

  delete[] ndata;
  return texture;

    
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ndata);
  delete[] ndata;

  // ... nice trilinear filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 

  bool use_hand_mipmap = false;
  if( use_hand_mipmap) {
    gdImagePtr scaled_image;

    int ix = 0;
    for(int divide_by=2; divide_by <= 16; divide_by *= 2) {
      scaled_image = make_scaled_image( image, 64, divide_by);
      ndata = make_texture_data( scaled_image);
      ix++;
      glTexImage2D(GL_TEXTURE_2D, ix, GL_RGBA, width/divide_by, height/divide_by, 0, GL_RGBA, GL_UNSIGNED_BYTE, ndata);
      delete[] ndata;
      gdImageDestroy( scaled_image);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, ix);
  }
  else {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  return texture;
}
  

GLuint TextureFactory::_get(std::string name)
{
  GLuint texture;
    
  if( m.find( name) == m.end()) {
    if( name == "terrain") {
      // texture = loadBMP_custom("/home/larry/mcpe-view/textures/terrain-atlas.bmp");
      texture = load("/home/larry/smooth/assets/images/terrain-atlas.png");
      m[name] = texture;
    }
  }

  return m[name];
}
