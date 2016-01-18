#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "common.h"
#include "database.h"
#include "drawable.h"
#include "packed_block.h"
#include "block_info.h"

class ColumnInfo {
 public:
  bool dirty_model;
  bool dirty_opacity;
  bool dirty_sides;
};

class TerrainChunk : public Drawable {
public:
  
  int x;
  int z;

  PackedBlockArray *pba;
  ColumnInfo column_info[16][16];
  
  class ColorShader *shader;
  class BlockShader *bshader;
  
  typedef enum {unloaded, loaded} LoadState;
  LoadState load_state;

  bool rebuild_opacity;
  bool rebuild_sides;
  bool rebuild_shader;
  
  TerrainChunk(int x, int z);

  void load();

  void setup();
  void draw(int pri=0);
  void teardown();

private:
  static LoggerPtr logger;

};


#endif
