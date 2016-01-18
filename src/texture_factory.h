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

  std::string readEntireFile(std::string filename);
  void loadBmp(std::string filename);

  char *make_texture_data(gdImagePtr image);
  char *make_texture_array_data(gdImagePtr image);

  gdImagePtr make_scaled_image(gdImagePtr master_image, int master_tile_size, int divide_by);

  

  int load(std::string path);
  GLuint _get(std::string name);
};


#endif
