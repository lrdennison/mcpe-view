#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"
#include "database.h"
#include "drawable.h"
#include "packed_block.h"
#include "block_info.h"
#include "terrain_chunk.h"


class Terrain : public Drawable, public Singleton<Terrain>
{
public:

  std::map< std::pair<int,int>, class TerrainChunk *> m;
    
  Terrain();

  static void scan_db()
  {
    Inst()._scan_db();
  }

  static TerrainChunk *get( int x, int z);

  static void get_column( int x, int z, PackedBlock *&pb, ColumnInfo *&ci);

  // Drawable routines
  void setup();
  void draw();
  void teardown();

  
protected:
  static LoggerPtr logger;
  
  void _scan_db();
  TerrainChunk *_get( int x, int z);
  void _get_column( int x, int z, PackedBlock *&pb, ColumnInfo *&ci);
  
};

#endif
