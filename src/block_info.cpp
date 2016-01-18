#include "block_info.h"
#include "texture_atlas.h"
#include "cube.h"

BlockInfo raw_block_info[] = {
  {0x00, "air", false, false},
  {0x01, "stone", true, true},
  {0x02, "grass", true, false},
  {0x03, "dirt", true, false},

  {0x04, "cobblestone", true, false},
  {0x05, "wooden_planks", true, false},
  {0x06, "sapling", false, false},
  {0x07, "bedrock", true, false},

  {0x08, "flowing_water", true, false},
  {0x09, "still_water", true, false},
  {0x0A, "lava", false, false},
  {0x0B, "stationary_lava", false, false},

  {0x0C, "sand", true, false},
  {0x0D, "gravel", true, false},
  {0x0E, "gold_ore", true, false},
  {0x0F, "iron_ore", true, false},

  {0x10, "coal_ore", true, false},
  {0x11, "log", true, false},	// aka wood
  {0x12, "leaves", false, false},
  {0x13, "sponge", true, false},

  //{0x14, "glass", false, false},
  {0x15, "lapis_lazuli_ore", true, false},
  {0x16, "lapis_lazuli_block", false, false},

  {0x18, "sandstone", true, false},
  
  {0x49, "redstone_ore", true, false},
  {0x4A, "redstone_block", true, false},

  {0x56, "pumpkin", true, false},
  {0x3C, "farmland", true, false},

  {52, "mob_spawner", true, false},
  {58, "crafting_table", true, false},
  {103, "melon", true, false},
};

Cube cube;


static void render_cube(Shader *shader, glm::mat4 &xform, unsigned int show_side, UVS *uvsa[6])
{
  for(int vx=0; vx<36; vx++) {
    int side = vx / 6;
    if( !(show_side & (0x1 << side)))
      continue;
	  
    glm::vec4 loc = xform * cube.xyz[vx];
    shader->vertices.push_back( loc);
    UV uv = cube.uv[vx];
    UV nuv = uv;
    
    shader->uv.push_back( nuv);

    shader->normal.push_back( cube.normal[vx]);

    //glm::vec2 tile( uvsa[side]->u0, uvsa[side]->v0);
    glm::vec2 tile( (float)uvsa[side]->tile_x, (float)uvsa[side]->tile_y);
    
    shader->tile.push_back( tile);

    assert( shader->vertices.size() == shader->uv.size());
    assert( shader->vertices.size() == shader->normal.size());
  }
}

  
static void render_generic(BLOCK_RENDER_ARGS)
{
  int id = bip->id;
  auto atlas_item = bip->atlas_item;
  if( !atlas_item)
    return;

  UVS uvs = atlas_item->uvs[0];

  UVS *uvsa[6];
  uvsa[0] = &uvs;
  uvsa[1] = &uvs;
  uvsa[2] = &uvs;
  uvsa[3] = &uvs;
  uvsa[4] = &uvs;
  uvsa[5] = &uvs;
 	
  render_cube(shader, xform, pb->show_side, uvsa);
}

#define PREAMBLE \
  int id = bip->id; \
  auto atlas_item = bip->atlas_item; \
  if( !atlas_item) \
    return; \
  UVS *uvsa[6];


static void render_grass(BLOCK_RENDER_ARGS)
{
  PREAMBLE;
    
  uvsa[0] = &atlas_item->uvs[3];
  uvsa[1] = &atlas_item->uvs[3];
  uvsa[2] = &atlas_item->uvs[3];
  uvsa[3] = &atlas_item->uvs[3];
  uvsa[4] = &atlas_item->uvs[2];
  uvsa[5] = &atlas_item->uvs[2];
 	
  render_cube(shader, xform, pb->show_side, uvsa);
}


static void render_leaves(BLOCK_RENDER_ARGS)
{
  PREAMBLE;

  int i = 4+pb->data;
    
  uvsa[0] = &atlas_item->uvs[i];
  uvsa[1] = &atlas_item->uvs[i];
  uvsa[2] = &atlas_item->uvs[i];
  uvsa[3] = &atlas_item->uvs[i];
  uvsa[4] = &atlas_item->uvs[i];
  uvsa[5] = &atlas_item->uvs[i];

  // printf( "leave data: %d\n", pb->data);
  render_cube(shader, xform, pb->show_side, uvsa);
}


static void render_log(BLOCK_RENDER_ARGS)
{
  PREAMBLE;

  int i = pb->data;
  i = i & 0x06;
  int alt = i & 0x01;
  
  AtlasItem *ai = bip->atlas_item;

  if( alt) {
    ai = bip->atlas_item2;
  }
    
  uvsa[0] = &ai->uvs[i];
  uvsa[1] = &ai->uvs[i];
  uvsa[2] = &ai->uvs[i];
  uvsa[3] = &ai->uvs[i];
  uvsa[4] = &ai->uvs[i+1];
  uvsa[5] = &ai->uvs[i+1];

  // printf( "log data: %d\n", pb->data);
  render_cube(shader, xform, pb->show_side, uvsa);
}


static void render_pumpkin(BLOCK_RENDER_ARGS)
{
  PREAMBLE;
    
  uvsa[0] = &atlas_item->uvs[2];
  uvsa[1] = &atlas_item->uvs[1];
  uvsa[2] = &atlas_item->uvs[1];
  uvsa[3] = &atlas_item->uvs[1];
  uvsa[4] = &atlas_item->uvs[0];
  uvsa[5] = &atlas_item->uvs[0];
 	
  render_cube(shader, xform, pb->show_side, uvsa);
}


static void render_use_data_as_uvs_index(BLOCK_RENDER_ARGS)
{
  PREAMBLE
    
  UVS uvs = atlas_item->uvs[pb->data];

  for(int ix=0; ix<6; ix++) {
    uvsa[ix] = &uvs;
  }
 	
  render_cube(shader, xform, pb->show_side, uvsa);
}




LoggerPtr BlockInfoBase::logger(Logger::getLogger("BlockInfoBase"));

BlockInfoBase::BlockInfoBase()
{
  for(int ix=0; ix<256; ix++) {
    m[ix] = 0;
  }

  for( auto b: raw_block_info) {
    BlockInfo *bip = new BlockInfo();
    *bip = b;
    m[b.id] = bip;
  }

  for( auto bip: m) {
    if( !bip)
      continue;

    // Default render function
    bip->render_func = render_generic;

    if( bip->name == "stone")
          bip->render_func = render_use_data_as_uvs_index;
    
    if( bip->name == "log")
          bip->render_func = render_log;
    
    if( bip->name == "leaves")
          bip->render_func = render_leaves;
    
    if( bip->name == "grass")
          bip->render_func = render_grass;
    
    if( bip->name == "pumpkin")
          bip->render_func = render_pumpkin;
    
    if( bip->name == "crafting_table")
          bip->render_func = render_pumpkin;
    
    if( terrain_atlas->find(bip->name) != terrain_atlas->end()) {
      bip->atlas_item = (*terrain_atlas)[bip->name];
      LOG4CXX_INFO( logger, "Found " << bip->name);
    }
    else {
      LOG4CXX_WARN( logger, "Couldn't find " << bip->name);
    }

    std::string s = bip->name + "2";
    if( terrain_atlas->find(s) != terrain_atlas->end()) {
      bip->atlas_item2 = (*terrain_atlas)[bip->name];
      LOG4CXX_INFO( logger, "Found " << s);
    }

    
  }
}



BlockInfo *BlockInfoBase::get( int id)
{
  return Inst()._get( id);
}

BlockInfo *BlockInfoBase::_get( int id)
{
  return m[id];
}
