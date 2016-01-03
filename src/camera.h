#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera {
 public:
  glm::vec3 location;
  GLfloat horizontalAngle = 3.14f;
  GLfloat verticalAngle = 0;

  int width;
  int height;

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 mvp;
  glm::vec3 direction;

  bool mode_2D;
  
  Camera();
  void update();

  
};

#endif
