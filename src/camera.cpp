#include "camera.h"


Camera::Camera()
{
  width = 1024;
  height = 768;
    
  location = glm::vec3(600,80,-140);
    
  GLfloat horizontalAngle = 3.14f;
  GLfloat verticalAngle = 0;

  mode_2D = false;

  update();
}


  
void Camera::update()
{
  direction.x = cos(verticalAngle) * sin(horizontalAngle);
  direction.y = sin(verticalAngle);
  direction.z = cos(verticalAngle) * cos(horizontalAngle);

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::radians(60.0f), (float) width / (float)height, 0.1f, 1000.0f);

  // Or, for an ortho camera :
  //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
  // Camera matrix

  glm::vec3 head_dir( 0, 1, 0);

  glm::mat4 View = glm::lookAt(
			       location, // Camera is at (4,3,3), in World Space
			       location+direction, // e origin
			       head_dir  // Head is up (set to 0,-1,0 to look upside-down)
			       );

 
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1.0f);

  // Our ModelViewProjection : multiplication of our 3 matrices
  mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

  if( mode_2D) {
    // left, right, bottom (y), top (y)
    mvp = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f,100.0f); // In world coordinates
  }
  
}
