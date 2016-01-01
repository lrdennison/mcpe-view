#ifndef VIEWER_H
#define VIEWER_H

#include "common.h"

#include "drawable.h"
#include "controller.h"

class Viewer
{

 public:
  bool glfw_is_up;
  GLFWwindow* window;
  GLuint VertexArrayID;

  int width;
  int height;

  Controller controls;
  
  std::vector<Drawable *> drawables;
  
  Viewer();

  void add( Drawable *d)
  {
    drawables.push_back( d);
  }
  
  
  void fatal( std::string msg)
  {
    fprintf( stderr, "%s\n", msg.c_str());
    getchar();
    throw Exc(msg);
  }
  

  // Various inits


  void init();
  int init_window();
  int init_glew();
  int init_vao();


  void run();
  
  virtual ~Viewer();
};

#endif
