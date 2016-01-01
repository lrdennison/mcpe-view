#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"
#include "camera.h"

class Controller {
 public:

  glm::mat4 mvp;

  GLFWwindow* window;

  GLfloat speed = 20.0f;

  double currentTime, lastTime, deltaTime;


  Controller()
  {
    update_time();
    mvp = camera->mvp;
  }

  void update_time()
  {
    lastTime = currentTime;
    currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);
  }
  

  void update()
  {
    update_time();
    
    if( glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS) {
      camera->location += glm::mat3(speed * deltaTime) * camera->direction;
    }
    if( glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS) {
      camera->location -= glm::mat3(speed * deltaTime) * camera->direction;
    }
      
    if( glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS) {
      camera->horizontalAngle -= 0.02;
    }
    if( glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS) {
      camera->horizontalAngle += 0.02;
    }
      
    if( glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS) {
      camera->location.y += 0.1;
    }
    if( glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS) {
      camera->location.y -= 0.1;
    }

    camera->update();
    mvp = camera->mvp;
  }
  
  
};

#endif
