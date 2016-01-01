#include "terrain.h"
#include "color_shader.h"
#include "block_shader.h"

LoggerPtr Terrain::logger(Logger::getLogger("Terrain"));

Terrain::Terrain()
{
}


void Terrain::_scan_db()
{
  leveldb::DB* db = Database::Inst().db;
  leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

  LOG4CXX_INFO( logger, "Starting database scan");
  int cnt = 0;
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::string key;

    key = it->key().ToString();
    int len = key.length();

    if( len == 9) {

      DbKey9 key;
      memcpy( (char *)&key, it->key().data(), 9);

      // Terrain data
      if( key.type == '0') {
	TerrainChunk *tc = new TerrainChunk( key.x, key.z);

	m[{key.x,key.z}] = tc;
	cnt++;

      }
    }

  }
  LOG4CXX_INFO( logger, "Created " << cnt << " terrain chunks");
  
  delete it;

}


TerrainChunk *Terrain::get( int x, int z)
{
    return Inst()._get(x, z);
}

TerrainChunk *Terrain::_get( int x, int z)
{

  if( m.find({x,z}) == m.end())
    return NULL;

  return m[{x,z}];
}


void Terrain::get_column( int x, int z, PackedBlock *&pb, ColumnInfo *&ci)
{
  Inst()._get_column( x, z, pb, ci);
}


void Terrain::_get_column( int x, int z, PackedBlock *&pb, ColumnInfo *&ci)
{
  pb = NULL;
  ci = NULL;
  
  int dx = x % 16; if(dx<0) dx+=16;
  int dz = z % 16; if(dz<0) dz+=16;

  // printf( "dx:%d dz:%d\n", dx, dz);

  int tc_x = (x-dx)/16;
  int tc_z = (z-dz)/16;

  assert( x == (tc_x * 16 + dx));
  assert( z == (tc_z * 16 + dz));

  TerrainChunk *tc = get(tc_x, tc_z);
  if( !tc || (tc->load_state != TerrainChunk::loaded))
    return;

  pb = &(tc->pba->b[dx][dz][0]);
  ci = &(tc->column_info[dx][dz]);
}


// Drawable routines


void Terrain::setup()
{
  for( auto tc : m) {
    tc.second->setup();
  }
}



void Terrain::draw()
{
  // Try to load chunks near the camera

  int cnt = 0;

  float camera_x = camera->location.x;
  float camera_z = camera->location.z;
  
  for( auto kv : m) {
    TerrainChunk *tc = kv.second;
    if( tc->load_state == TerrainChunk::loaded)
      continue;

    float lx = 16*tc->x;
    float lz = 16*tc->z;
    float ux = lx+16;
    float uz = lz+16;

    int extra=16*4;
    lx = lx-extra;
    lz = lz-extra;
    ux = ux+extra;
    uz = uz+extra;

    if( lx<=camera_x && camera_x<=ux &&
	lz<=camera_z && camera_z<=uz) {
      tc->load();

      TerrainChunk *ntc;
      ntc = get( tc->x+1, tc->z);
      if( ntc) {
	ntc->rebuild_opacity = true;
	ntc->rebuild_sides = true;
      }
      ntc = get( tc->x-1, tc->z);
      if( ntc) {
	ntc->rebuild_opacity = true;
	ntc->rebuild_sides = true;
      }
      ntc = get( tc->x, tc->z+1);
      if( ntc) {
	ntc->rebuild_opacity = true;
	ntc->rebuild_sides = true;
      }
      ntc = get( tc->x, tc->z-1);
      if( ntc) {
	ntc->rebuild_opacity = true;
	ntc->rebuild_sides = true;
      }
    }
  }

  if( cnt) {
    LOG4CXX_INFO( logger,"Loaded " << cnt << " terrain chunks");
    cnt = 0;
  }


  for( auto tuple : m) {
    TerrainChunk *tc = tuple.second;
    if( tc->load_state == TerrainChunk::loaded) {
      if( tc->rebuild_opacity) {
	tc->pba->update_opaque_hint();
	tc->rebuild_opacity = false;
	cnt++;
      }
    }
  }
  if( cnt) {
    LOG4CXX_INFO( logger,"Updated opacity on " << cnt << " terrain chunks");
    cnt = 0;
  }

  cnt = 0;
  for( auto tuple : m) {
    TerrainChunk *tc = tuple.second;
    if( tc->load_state == TerrainChunk::loaded) {
      if( tc->rebuild_sides) {
	tc->pba->update_show_sides();
	tc->rebuild_sides = false;
	cnt++;
      }
    }
  }
  if( cnt) {
    LOG4CXX_INFO( logger, "Updated side visibility on " << cnt << " terrain chunks");
    cnt = 0;
  }
  

  cnt = 0;
  for( auto tuple : m) {
    TerrainChunk *tc = tuple.second;
    if( tc->load_state == TerrainChunk::loaded) {
      if( tc->rebuild_shader) {
	tc->setup();
	tc->rebuild_shader = false;
	cnt++;
      }
    }
  }
  if( cnt) {
    LOG4CXX_INFO( logger, "Updated shader on " << cnt << " terrain chunks");
    cnt = 0;
  }

  for( auto tc : m) {
    tc.second->draw();
  }

}
  
void Terrain::teardown()
{
  for( auto tc : m) {
    tc.second->teardown();
  }
}
