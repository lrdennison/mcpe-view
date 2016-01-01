#ifndef BLOCK_INFO_H
#define BLOCK_INFO_H

#include "common.h"
#include "packed_block.h"
#include "terrain_chunk.h"

#define BLOCK_RENDER_ARGS Vertices &v_out, UVs &uv_out, glm::mat4 &xform, class BlockInfo *bip, PackedBlock *pb, ColumnInfo *ci
typedef void BlockRenderFunc( BLOCK_RENDER_ARGS );


struct BlockInfo
{
  unsigned int id;
  std::string name;
  bool is_solid;
  bool use_data_as_uvs_index;

  // From the terrain atlas
  class AtlasItem *atlas_item;
  class AtlasItem *atlas_item2;
  BlockRenderFunc *render_func;
};


class BlockInfoBase : public Singleton<BlockInfoBase>
{
public:
  BlockInfoBase();

  static BlockInfo *get( int id);
  
protected:
  static LoggerPtr logger;
  
  BlockInfo *_get( int id);

  BlockInfo *m[256];
};
  

#endif
