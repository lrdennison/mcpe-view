#include "block_info.h"
#include "texture_atlas.h"
#include "cube.h"

#include "block_render.h"

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


static void render_sapling(BLOCK_RENDER_ARGS)
{
  PREAMBLE;

  int i = pb->data;
    
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


BlockRenderFunc *get_block_renderer( std::string name)
{
  BlockRenderFunc *func = render_generic;

  if( name == "stone")
    func = render_use_data_as_uvs_index;
    
  if( name == "log")
    func = render_log;
    
  if( name == "leaves")
    func = render_leaves;
    
  if( name == "sapling")
    func = render_sapling;
    
  if( name == "grass")
    func = render_grass;
    
  if( name == "pumpkin")
    func = render_pumpkin;
    
  if( name == "crafting_table")
    func = render_pumpkin;

  return func;
}

