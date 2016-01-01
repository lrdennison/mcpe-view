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
