#ifndef PACKED_BLOCK_H
#define PACKED_BLOCK_H

// Packed blocks are a dense representation of a block
// suitable for processing

struct PackedBlock
{
  unsigned int id : 8;
  unsigned int data : 4;
  unsigned int skylight : 4;
  unsigned int block_light : 4;

  // Additions over what is kept in the database
  
  unsigned int opaque_hint : 1;
  unsigned int show_side : 6;

  static const
  unsigned int zp=0x01,
    xp=0x02,
    zm=0x04,
    xm=0x08,
    ym=0x10,
    yp=0x20,
    all=0x3F,
    none=0x00;
};


// A chunk's worth of packed blocks.
// The array dimensions are [X][Z][Y]

class PackedBlockArray {
 public:
  class TerrainChunk *parent;
  
  int x, z;
  PackedBlock b[16][16][128];


  PackedBlockArray( TerrainChunk *parent, int x, int z, class DbTerrain *dbt);
  ~PackedBlockArray();

  PackedBlock *getBuddy( int bx, int bz, int by);
  
  void update_opaque_hint();
  void update_show_sides();
};


#endif
