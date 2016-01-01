#include "common.h"
#include "viewer.h"
#include "drawable.h"

void HUDDrawable::setup()
{
  colorShader = new ColorShader();
  // colorShader->move_data_to_buffers();

  gdShader = new GdShader(512, 128);
  gdShader->x = 10.0;
  gdShader->y = 10.0;
  
  gdShader->move_data_to_buffers();
}



void HUDDrawable::draw()
{
  int white=gdTrueColor(255,255,255);
  int fun=gdTrueColor(0,200,0);
  
  int width = gdShader->image->sx;
  int height = gdShader->image->sy;

  gdImageFilledRectangle( gdShader->image, 0, 0, width, height, white);
  char* err;
  char s[256];
  sprintf(s, "x:%0.1f y:%0.1f z:%0.1f",
	  camera->location.x,
	  camera->location.y,
	  camera->location.z);
    
  double sz = 12.0;
  char *f = (char *)"/usr/share/fonts/dejavu/DejaVuSansMono.ttf";  /* User supplied font */
  int brect[8];

  /* obtain brect so that we can size the image */
  err = gdImageStringFT(gdShader->image,&brect[0],fun,f,sz,0.,10,100,s);

  // colorShader->draw();
  gdShader->move_data_to_buffers();
  gdShader->draw();
}


void HUDDrawable::teardown()
{
}
