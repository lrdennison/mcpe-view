#include "common.h"
#include "viewer.h"
#include "drawable.h"

#include <math.h>

using namespace glm;

Viewer::Viewer()
{
  VertexArrayID = 0;
    
  glfw_is_up = false;
  window = NULL;
  width = 1024;
  height = 768;
}


Viewer::~Viewer()
{ 
  if( VertexArrayID != 0) {
    glDeleteVertexArrays(1, &VertexArrayID);
  }

  // Close OpenGL window and terminate GLFW
  if( glfw_is_up)
    glfwTerminate();

  
}


void Viewer::init()
{
  init_window();
  init_glew();
  init_vao();
}

static void resize_callback( GLFWwindow *window, int width, int height)
{
  printf( "Resize to %d x %d\n", width, height);

  glfwGetFramebufferSize(window, &width, &height);
 
  glViewport( 0, 0, width, height);

  camera->width = width;
  camera->height = height;
}


int Viewer::init_window()
{
  // Initialise GLFW
  if( !glfwInit()) {
      fatal( "Failed to initialize GLFW" );
  }
  glfw_is_up = true;
  
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( width, height, "MCPE Viewer", NULL, NULL);
  if( window == NULL ) {
    fatal( "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." );
  }
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, resize_callback);
			    
  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  return 0;
}


int Viewer::init_glew()
{
  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fatal("Failed to initialize GLEW");
  }

  return 0;
}


int Viewer::init_vao()
{
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  return 0;
}



void mode_3D()
{
  glDisable(GL_BLEND);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  camera->mode_2D = false;
  camera->update();
}

void mode_2D()
{
  // Enable depth test
  glDisable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  camera->mode_2D = true;
  camera->update();
}


void Viewer::run()
{
  for( auto d : drawables) {
    d->setup();
  }
  
  controls.window = window;
 
  do {

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    controls.update();

    mode_3D();
    for( auto d : drawables) {
      if( d->ui_component == Drawable::world)
	d->draw();
    }

    mode_2D();
    for( auto d : drawables) {
      if( d->ui_component == Drawable::hud)
	d->draw();
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    
  }

  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );


  for( auto d : drawables) {
    d->teardown();
  }

}

