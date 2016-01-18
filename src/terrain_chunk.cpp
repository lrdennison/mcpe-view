#include "terrain.h"
#include "color_shader.h"
#include "block_shader.h"

LoggerPtr TerrainChunk::logger(Logger::getLogger("TerrainChunk"));

TerrainChunk::TerrainChunk(int x, int z)
{
  this->x = x;
  this->z = z;
  load_state = unloaded;
  pba = NULL;

  shader = NULL;
  bshader = NULL;
}


void TerrainChunk::load()
{
  if( load_state == loaded)
    return;

  std::string s;
  Database::get( s, x, z, '0');
  // printf( "Got %d bytes\n", s.length());
  // printf( "Need %d bytes\n", sizeof(DbTerrain));

  assert( sizeof(DbTerrain) == s.length() );

  DbTerrain *dbt = (DbTerrain *)s.c_str();

  pba = new PackedBlockArray(this, x, z, dbt);

  load_state = loaded;
  rebuild_opacity = true;
  rebuild_sides = true;
  rebuild_shader = true;
}


void TerrainChunk::setup()
{
  if( load_state != loaded)
    return;

  if( !shader)
    shader = new ColorShader();

  if( !bshader)
    bshader = new BlockShader();
  
  glm::vec3 red(1.0, 0.0, 0.0);
  glm::vec3 blue(0.0, 1.0, 1.0);
  glm::vec3 gray(0.2, 0.2, 0.2);

  float lx = x*16;
  float ux = lx+16.0;
  float lz = z*16;
  float uz = lz+16.0;
  float y = 64.0;
  
  shader->vertices.push_back( glm::vec4(lx, y, lz, 1.0));
  shader->vertices.push_back( glm::vec4(ux, y, lz, 1.0));
  shader->vertices.push_back( glm::vec4(lx, y, uz, 1.0));
    
  shader->vertices.push_back( glm::vec4(ux, y, uz, 1.0));
  shader->vertices.push_back( glm::vec4(ux, y, lz, 1.0));
  shader->vertices.push_back( glm::vec4(lx, y, uz, 1.0));
    
  glm::vec3 col = blue;
    
  for( int ix=0; ix<6; ix++) {
    shader->colors.push_back( red);
  }

  assert( bshader->vertices.size() == bshader->uv.size());
  assert( bshader->vertices.size() == bshader->normal.size());

  int ix=0;
  for( int dx=0; dx<16; dx++) {
    for( int dz=0; dz<16; dz++) {
      for( int dy=0; dy<128; dy++) {
	PackedBlock *pb = &(pba->b[dx][dz][dy]);
	if( !pb->show_side)
	  continue;
	
	int id = pb->id;
	
	auto bip = BlockInfoBase::get(id);
	if( !bip)
	  continue;

	glm::vec3 pos((float)(x*16+dx), (float)dy, (float)(z*16+dz));
	glm::mat4 xform = glm::mat4( 1.0f);
	xform = glm::translate( xform, pos);

	bip->render_func( bshader, xform, bip, pb, 0);

	int priority = 3;
	if(bip->is_solid) {
	  priority=0;
	}
	
	while( bshader->priority.size() < bshader->vertices.size()) {
	  bshader->priority.push_back( priority);
	}
	
      }
    }
  }


  shader->move_data_to_buffers();

  assert( bshader->vertices.size() == bshader->uv.size());
  assert( bshader->vertices.size() == bshader->normal.size());
  bshader->move_data_to_buffers();
}

void TerrainChunk::draw(int pri)
{
  if( load_state != loaded)
    return;
  
  //shader->draw();
  bshader->draw(pri);
}

void TerrainChunk::teardown()
{
}

