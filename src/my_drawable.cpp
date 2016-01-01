#include "viewer.h"
#include "drawable.h"
#include "cube.h"
#include "block.h"

using namespace glm;


void MyDrawable::setup()
{
  vs.textureShader = new BlockShader();
  
  Block b1, b2;

  glm::vec3 t(-2.0,0.0,0.0);
  
  b2.xform = glm::translate( b2.xform, t);
  b2.render( vs);
  b1.render( vs);

  for( int ix=0; ix<blocks.size(); ix++) {
    blocks[ix]->render( vs);
  }

  // colorShader->move_data_to_buffers();
  vs.textureShader->move_data_to_buffers();

}




void MyDrawable::draw()
{
  // colorShader->draw();
  vs.textureShader->draw();

}


void MyDrawable::teardown()
{
}

