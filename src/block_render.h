#ifndef BLOCK_RENDER_H
#define BLOCK_RENDER_H

#include "common.h"
#include "packed_block.h"
#include "terrain_chunk.h"

#define BLOCK_RENDER_ARGS Shader *shader, glm::mat4 &xform, class BlockInfo *bip, PackedBlock *pb, ColumnInfo *ci
typedef void BlockRenderFunc( BLOCK_RENDER_ARGS );

BlockRenderFunc *get_block_renderer( std::string name);

#endif
