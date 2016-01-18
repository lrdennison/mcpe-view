#include <math.h>
#include "texture_atlas.h"


using namespace std;

LoggerPtr TextureAtlas::logger(Logger::getLogger("TextureAtlas"));

float TextureAtlas::parse_number()
{
  float f;
  fp >> f;
  // printf( "number: %f\n", f);
  return f;
}

string TextureAtlas::parse_string()
{
  string s("");
  char c;
  
  c = fp.get();
  c = fp.get();
  while( c != '"') {
    s += c;
    c = fp.get();
  }
  return s;
}

void TextureAtlas::parse_uvs()
{
  char c;
  UVS uvs;

  c = fp.get();
  c = fp.peek();

  uvs.u0 = parse_number();
  c = fp.get(); assert( c == ',');
  uvs.v0 = parse_number();
  c = fp.get(); assert( c == ',');
  uvs.u1 = parse_number();
  c = fp.get();  assert( c == ',');
  uvs.v1 = parse_number();
  c = fp.get(); assert( c == ',');
  uvs.s0 = parse_number(); 
  c = fp.get();  assert( c == ',');
  uvs.s1 = parse_number();
  c = fp.get(); assert( c == ']');

  m->uvs.push_back( uvs);
}

void TextureAtlas::parse_uvs_array()
{
  char c;
  c = fp.get();
  c = fp.peek();
  while( c != ']') {
    parse_uvs();
    c = fp.peek();
    if( c == ',')
      c = fp.get();
    c = fp.peek();
  }
  c = fp.get();
  
}


void TextureAtlas::parse_item()
{
  char c;
  string s;

  c = fp.peek();
  // Structure
  if( c == '{') {
    c = fp.get();
    
    m = new AtlasItem;
    
    s = parse_string(); // name
    fp.get(); // colon
    m->name = parse_string();
    fp.get(); // comma
    s = parse_string(); // uvs
    fp.get(); // colon
    parse_uvs_array();

    c = fp.get(); assert( c == '}');

    (*this)[m->name] = m;

    LOG4CXX_DEBUG( logger, "name: " << m->name);
  }
}

void TextureAtlas::parse_file()
{
  if( fp.eof())
    return;

  char c = fp.peek();

  // Array
  if( c == '[') {
    c = fp.get();

    while( true) {
      TextureAtlas::parse_item();
      c = fp.get();
      if( c == ',') {
	continue;
      }
      if( c == ']') {
	break;
      }
      printf( "Parse failed.\n");
      break;
    }
  }


  
}

static float snap(float n_in)
{
  float n_out = n_in * 32.0f;
  n_out = round( n_out);
  n_out = n_out / 32.0f;

  float error = n_in - n_out;
  return n_out;
}

void TextureAtlas::load()
{
  string path = "/home/larry/smooth/assets/images/terrain.meta";
  LOG4CXX_INFO( logger, "Loading from " << path);
  fp.open(path, std::ios::in);
  if(fp.is_open()){

    TextureAtlas::parse_file();
    fp.close();
  }
  else {
    printf("Impossible to open %s. Are you in the right directory ?\n", path);
    getchar();
  }

  for( auto tuple : (*this)) {
    AtlasItem *mi = tuple.second;
    LOG4CXX_INFO( logger, "name " << mi->name << "[" << mi->uvs.size() << "]");
    
    for( int i=0; i<mi->uvs.size(); i++ ) {
      UVS *uvs = &mi->uvs[i];

      uvs->tile_x = round( snap( uvs->u0) * 32.0f);
      uvs->tile_y = round( snap( uvs->v0) * 16.0f);
      
      uvs->u0 = snap( uvs->u0) + 0.5/2048.0;
      uvs->u1 = snap( uvs->u1) - 0.5/2048.0;
      uvs->v0 = snap( uvs->v0) + 0.5/1024.0;
      uvs->v1 = snap( uvs->v1) - 0.5/1024.0;
    }
  }
  
  LOG4CXX_INFO( logger, "Finished loading");
}
