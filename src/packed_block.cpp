#include "common.h"
#include "packed_block.h"
#include "block_info.h"
#include "database.h"
#include "terrain.h"
#include "terrain_chunk.h"

// Remember all packed arrays so they can
// directly find neighbors

static std::map< std::pair<int,int>, PackedBlockArray *> pbas;


PackedBlockArray::PackedBlockArray(TerrainChunk *parent, int x, int z, class DbTerrain *dbt)
{
  this->parent = parent;
  this->x = x;
  this->z = z;
  
  int ix=0;
  for( int dx=0; dx<16; dx++) {
    for( int dz=0; dz<16; dz++) {

      // On load, we mark all columns as dirty
      parent->column_info[dx][dz].dirty_model = true;
      parent->column_info[dx][dz].dirty_opacity = true;
      parent->column_info[dx][dz].dirty_sides = true;
      
      for( int dy=0; dy<128; dy++) {

	PackedBlock &pb = b[dx][dz][dy];

	unsigned char dv = dbt->dv[ix>>1];
	if( ix & 0x1) {
	  dv = dv >> 4;
	}

	pb.id  = dbt->block[ix];
	pb.data = dv & 0xF;
	pb.skylight = 0;
	pb.block_light = 0;
	pb.opaque_hint = true;
	pb.show_side = PackedBlock::all;
	
	ix++;
      }
    }
  }

  update_opaque_hint();
  
  // Remember ourselves
  pbas[{x,z}] = this;
}


PackedBlockArray::~PackedBlockArray()
{
}


void PackedBlockArray::update_opaque_hint()
{
  for(int ix=0; ix<16; ix++) {
    for(int iz=0; iz<16; iz++) {
      bool changed = false;

      // if( !parent->column_info[ix][iz].dirty_model && !parent->rebuild_opacity)
      // 	continue;
      
      for(int iy=0; iy<128; iy++) {
	PackedBlock &pb = b[ix][iz][iy];
	BlockInfo *bip = BlockInfoBase::get( pb.id);

	bool prior = pb.opaque_hint;
	pb.opaque_hint = false;
	if( bip && bip->is_solid)
	  pb.opaque_hint = true;
	if( pb.opaque_hint != prior)
	  changed = true;
      }
 
      parent->column_info[ix][iz].dirty_opacity = changed;
      parent->rebuild_sides |= changed;
    }
  }
}


PackedBlock *PackedBlockArray::getBuddy( int bx, int bz, int by)
{
  int tx = x;
  int tz = z;

  while( bx<0) {
    bx += 16;
    tx -= 1;
  }
  while( bx>=16) {
    bx -= 16;
    tx += 1;
  }
  while( bz<0) {
    bz += 16;
    tz -= 1;
  }
  while( bz>=16) {
    bz -= 16;
    tz += 1;
  }

  if( tx==x && tz==z)
    return &b[bx][bz][by];

  if( pbas.find({tx,tz}) == pbas.end())
    return 0;

  PackedBlockArray *pba = pbas[{tx,tz}];

  return &(pba->b[bx][bz][by]);
}


struct {
  int dx;
  int dz;
  unsigned int show_bit;
} buddies[] = {
  {-1, 0,PackedBlock::xm},
  { 1, 0,PackedBlock::xp},
  { 0,-1,PackedBlock::zm},
  { 0, 1,PackedBlock::zp},
};


void PackedBlockArray::update_show_sides()
{
  for(int ix=0; ix<16; ix++) {
    for(int iz=0; iz<16; iz++) {

      // printf( "x:%d z:%d\n", ix, iz);

      bool dirty_opacity = parent->column_info[ix][iz].dirty_opacity;
      dirty_opacity=true;
      bool changed = false;
    
      if( dirty_opacity) {
	for(int iy=0; iy<128; iy++) {
	  PackedBlock &pb = b[ix][iz][iy];
	  unsigned int prior = pb.show_side;
	  pb.show_side |= PackedBlock::yp | PackedBlock::ym;
	  
	  // No need for bottom of bedrock
	  if( iy==0 ) {
	    pb.show_side &= ~PackedBlock::ym;
	  }

	  if(iy+1 < 128) {
	      PackedBlock &buddy = b[ix][iz][iy+1];
	    if( buddy.opaque_hint)
	      pb.show_side &= ~PackedBlock::yp;
	  }
	  if(iy-1 >= 0) {
	    PackedBlock &buddy = b[ix][iz][iy-1];
	    if( buddy.opaque_hint)
	      pb.show_side &= ~PackedBlock::ym;
	  }
	  if( pb.show_side != prior)
	    changed = true;
	}
      }


      for( auto buddy : buddies) {
	PackedBlock *pba;
	ColumnInfo *ci;
	int tx = ix+buddy.dx;
	int tz = iz+buddy.dz;
	
	Terrain::get_column( x*16+tx, z*16+tz, pba, ci);

	if(tx>=0 && tx<16 && tz>=0 && tz<16) {
	  assert( pba == &b[tx][tz][0]);
	}

	if( !dirty_opacity) {
	  if( !ci || !ci->dirty_opacity)
	    continue;
	}

	
	for(int iy=0; iy<128; iy++) {
	  PackedBlock &pb = b[ix][iz][iy];
	  bool buddy_is_opaque = !pba || pba[iy].opaque_hint;
	  
	  unsigned int prior = pb.show_side;
	  
	  pb.show_side |= buddy.show_bit;

	  if( buddy_is_opaque && pb.opaque_hint) {
	    pb.show_side &= ~buddy.show_bit;
	  }
	  
	  if( pb.show_side != prior)
	    changed = true;
	}
      }

      parent->column_info[ix][iz].dirty_opacity = false;
      parent->column_info[ix][iz].dirty_sides = changed;
      parent->rebuild_shader |= changed;
    }
  }
}
