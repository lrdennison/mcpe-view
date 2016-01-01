#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "common.h"
#include "vertex_stream.h"
#include "block.h"
#include "shader_program.h"
#include "color_shader.h"
#include "block_shader.h"
#include "gd_shader.h"

class Drawable {
 public:
  typedef enum {world, hud} UIComponent;
  UIComponent ui_component;

  Drawable()
  {
    ui_component = world;
  }

  virtual void setup() {}
  virtual void draw() {}
  virtual void teardown() {}
};



class MyDrawable : public Drawable {
 public:
  std::vector<Block *> blocks;

  VertexStream vs;
  
  void setup();
  void draw();
  void teardown();
};


class GridDrawable : public Drawable {
 public:
  ColorShader *colorShader;
  
  void setup();
  void draw();
  void teardown();
};

class HUDDrawable : public Drawable {
 public: 
  ColorShader *colorShader;
  GdShader *gdShader;
 
  HUDDrawable()
  {
    ui_component = hud;
  }

  void setup();
  void draw();
  void teardown();
};




#endif
