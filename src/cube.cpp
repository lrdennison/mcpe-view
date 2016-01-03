#include "common.h"
#include "cube.h"
#include "vertex_stream.h"

void Cube::mk_face( GLfloat d0[], GLfloat d1[], GLfloat d2[], GLfloat d2v)
{
  float ld = -0.50;
  float ud =  0.50;

  float luv = 0.00;
  float uuv = 1.00;
  
  d0[cnt] = ld;
  d1[cnt] = ld;
  d2[cnt] = d2v;
  u[cnt] = luv;
  v[cnt] = luv;
  cnt++;
  d0[cnt] = ud;
  d1[cnt] = ld;
  d2[cnt] = d2v;
  u[cnt] = uuv;
  v[cnt] = luv;
  cnt++;
  d0[cnt] = ld;
  d1[cnt] = ud;
  d2[cnt] = d2v;
  u[cnt] = luv;
  v[cnt] = uuv;
  cnt++;
    
  d0[cnt] = ud;
  d1[cnt] = ud;
  d2[cnt] = d2v;
  u[cnt] = uuv;
  v[cnt] = uuv;
  cnt++;
  d0[cnt] = ud;
  d1[cnt] = ld;
  d2[cnt] = d2v;
  u[cnt] = uuv;
  v[cnt] = luv;
  cnt++;
  d0[cnt] = ld;
  d1[cnt] = ud;
  d2[cnt] = d2v;
  u[cnt] = luv;
  v[cnt] = uuv;
  cnt++;

  face_cnt++;
}

class FV {
public:
  Vertex vertex;
  UV uv;
  glm::vec4 normal;
};

//The unit face should have a normal towards Z+
static FV unit_face[] = {
  // x    y    z    w     u    v
  {{0.0, 0.0, 0.0, 1.0},  {0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}},
  {{1.0, 0.0, 0.0, 1.0},  {1.0, 0.0}, {0.0, 0.0, 1.0, 0.0}},
  {{1.0, 1.0, 0.0, 1.0},  {1.0, 1.0}, {0.0, 0.0, 1.0, 0.0}},

  // x    y    z    w     u    v
  {{1.0, 1.0, 0.0, 1.0},  {1.0, 1.0}, {0.0, 0.0, 1.0, 0.0}},
  {{0.0, 1.0, 0.0, 1.0},  {0.0, 1.0}, {0.0, 0.0, 1.0, 0.0}},
  {{0.0, 0.0, 0.0, 1.0},  {0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}},
};


Cube::Cube()
{
  cnt = 0;
  face_cnt = 0;
    
  mk_face( z, y, x, -0.5);
  mk_face( z, y, x,  0.5);
  mk_face( x, y, z, -0.5);
  mk_face( x, y, z,  0.5);
  mk_face( x, z, y, -0.5);
  mk_face( x, z, y,  0.5);

  for(int ix=0; ix<36; ix++) {
    xyz[ix].x = x[ix];
    xyz[ix].y = y[ix];
    xyz[ix].z = z[ix];
    xyz[ix].w = 1.0f;
      
    uv[ix].x = u[ix];
    uv[ix].y = v[ix];
  }

  static bool print_it = true;
  // New way
  FV reference_face[6];
  glm::vec3 loc(-0.5f,-0.5f, 0.5f);
  glm::mat4 mat(1.0f);
  mat = glm::translate(mat, loc);

  for( int ix=0; ix<6; ix++) {
    reference_face[ix] = unit_face[ix];
    Vertex v = reference_face[ix].vertex;
    v = mat*v;
    reference_face[ix].vertex = v;

    if( print_it) {
      printf("ref %d: %f %f %f\n", ix, v.x, v.y, v.z);
    }
  }

  glm::vec3 axis_x(1, 0, 0);
  glm::vec3 axis_y(0, 1, 0);
  glm::vec3 axis_z(0, 0, 1);

  FV all[36];
  int cnt=0;
  for(int side=0; side<6; side++) {
    if( side < 4)
      mat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f*side), axis_y);
    if( side == 4)
      mat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), axis_x);
    if( side == 5)
      mat = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), axis_x);

    for( int ix=0; ix<6; ix++) {
      all[cnt] = reference_face[ix];
      Vertex v = all[cnt].vertex;
      v = mat*v;
      all[cnt].vertex = v;

      all[cnt].normal = mat*all[cnt].normal;
      
      if( print_it) {
	printf("all %d: %f %f %f\n", cnt, v.x, v.y, v.z);
      }
      xyz[cnt] = all[cnt].vertex;
      uv[cnt] = all[cnt].uv;
      normal[cnt].x = all[cnt].normal.x;
      normal[cnt].y = all[cnt].normal.y;
      normal[cnt].z = all[cnt].normal.z;
      cnt++;
    }
  }
 
  print_it = false;
}


void Cube::render( VertexStream &vs, glm::mat4 &xform, UVS &uvs)
{
  for(int ix=0; ix<36; ix++) {
    glm::vec4 loc = xform * xyz[ix];
    vs.textureShader->vertices.push_back( loc);

    vec2 nuv = uv[ix];

    if( uv[ix].x < 0.1)
      nuv.x = uvs.u0;
    if( uv[ix].x > 0.9)
      nuv.x = uvs.u1;
    if( uv[ix].y < 0.1)
      nuv.y = 1.0 - uvs.v0;
    if( uv[ix].y > 0.9)
      nuv.y = 1.0 - uvs.v1;
    
    vs.textureShader->uv.push_back( nuv);
  }
}
