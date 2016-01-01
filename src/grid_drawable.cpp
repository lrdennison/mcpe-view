#include "viewer.h"
#include "drawable.h"

void GridDrawable::setup()
{
  colorShader = new ColorShader();

  glm::vec3 red(1.0, 0.0, 0.0);
  glm::vec3 blue(0.0, 0.0, 1.0);
  glm::vec3 gray(0.2, 0.2, 0.2);

  for( int dx=-100*16; dx<=100*16; dx+=16) {
    float x = (float )dx;
    colorShader->vertices.push_back( glm::vec4(x,  0.0, -5000.0, 1.0));
    colorShader->vertices.push_back( glm::vec4(x, -1.0, -5000.0, 1.0));
    colorShader->vertices.push_back( glm::vec4(x,  0.0,  5000.0, 1.0));
    
    colorShader->vertices.push_back( glm::vec4(x, -1.0, -5000.0, 1.0));
    colorShader->vertices.push_back( glm::vec4(x, -1.0,  5000.0, 1.0));
    colorShader->vertices.push_back( glm::vec4(x,  0.0,  5000.0, 1.0));
    
    glm::vec3 col = gray;
    if( dx==0)
      col = red;
    
    for( int ix=0; ix<6; ix++) {
      colorShader->colors.push_back( col);
    }

    colorShader->vertices.push_back( glm::vec4(-5000.0,  0.0, x, 1.0));
    colorShader->vertices.push_back( glm::vec4(-5000.0, -1.0, x, 1.0));
    colorShader->vertices.push_back( glm::vec4( 5000.0,  0.0, x, 1.0));
    
    colorShader->vertices.push_back( glm::vec4(-5000.0, -1.0, x, 1.0));
    colorShader->vertices.push_back( glm::vec4( 5000.0, -1.0, x, 1.0));
    colorShader->vertices.push_back( glm::vec4( 5000.0,  0.0, x, 1.0));

    col = gray;
    if( dx==0)
      col = blue;
    
    for( int ix=0; ix<6; ix++) {
      colorShader->colors.push_back( col);
    }
  }

  colorShader->move_data_to_buffers();
}




void GridDrawable::draw()
{
  colorShader->draw();
}


void GridDrawable::teardown()
{
}
