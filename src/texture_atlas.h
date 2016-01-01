#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "common.h"

struct UVS {
  float u0, v0, u1, v1, s0, s1;
};

struct AtlasItem {
  std::string name;
  std::vector<UVS> uvs;
};

class TextureAtlas : public std::map<std::string, AtlasItem *> {

public:

  void load();

private:
  static LoggerPtr logger;
  
  std::ifstream fp;
  AtlasItem *m;

  void parse_file();
  void parse_item();
  void parse_uvs_array();
  void parse_uvs();
  
  float parse_number();
  std::string parse_string();
};

#endif
