#include "block.h"

Cube *Block::cube = new Cube();

Block::Block()
{
  xform = glm::mat4( 1.0f);
}

void Block::render( VertexStream &vs)
{
  UVS uvs;
  uvs.u0 = 0.0;
  uvs.u1 = 1.0;
  uvs.v0 = 0.0;
  uvs.v1 = 1.0;

  std::string s = "cobblestone";
  int ix = 0;
  
  if( id == 0x01) {
    s = "stone";
    ix = data;
  }

  if( id == 0x34) {
    s = "mob_spawner";
    // ix = data;
  }

  if( id == 0x0E) {
    s = "gold_ore";
    // ix = data;
  }

  if( id == 0x38) {
    s = "diamond_ore";
    // ix = data;
  }

  if( terrain_atlas->find(s) != terrain_atlas->end() ) {
    AtlasItem *m = (*terrain_atlas)[s];
    if( m) {
      uvs = m->uvs[ix];
      /*
      printf( "found %f %f %f %f\n", uvs.u0, uvs.v0, uvs.u1, uvs.v1);
      uvs.u0 = 0.0*64 / 2048.0;
      uvs.u1 = 1.0*64 / 2048.0;
      uvs.v0 = 0.0*64 / 1024.0;
      uvs.v1 = 1.0*64 / 1024.0;
      */
    }
  }

  
  cube->render( vs, xform, uvs);
}
