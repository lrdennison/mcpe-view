#include "shader.h"
#include "camera.h"


Shader::Shader()
{
  program = 0;
  _color = false;
  _normal = false;
  _uv = false;


  glGenBuffers(1, &vertexbuffer);
}


void Shader::use_program(std::string name)
{
  program = ShaderProgramFactory::get(name);

  // Now that we have the program, find
  // the uniform memory locations
  
  mvp_uloc = glGetUniformLocation(program->id, "MVP");
  m_uloc = glGetUniformLocation(program->id, "M");
  v_uloc = glGetUniformLocation(program->id, "V");
  texture_uloc = glGetUniformLocation(program->id, "myTextureSampler");
}


void Shader::use_color()
{
  _color = true;
  glGenBuffers(1, &colorbuffer);
}

void Shader::use_normal()
{
  _normal = true;
  glGenBuffers(1, &normalbuffer);
}

void Shader::use_uv()
{
  _uv = true;
  glGenBuffers(1, &uvbuffer);
}



void Shader::use_texture()
{
  use_uv();
  _texture = true;
}



void Shader::move_data_to_buffers()
{
  num_vertices = vertices.size();
  
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);

  if( _uv) {
    assert( uv.size() == num_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
  }

  if( _color) {
    assert( color.size() == num_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), &color[0], GL_STATIC_DRAW);
  }

  if( _normal) {
    assert( normal.size() == num_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
  }
  
  vertices.clear();
  uv.clear();
  color.clear();
  normal.clear();
}
 


void Shader::draw()
{
  glUseProgram(program->id);

  if( mvp_uloc != -1) {
    glUniformMatrix4fv(mvp_uloc, 1, GL_FALSE, &(camera->mvp[0][0]) );
  }
  
  if( m_uloc != -1) {
    glUniformMatrix4fv(m_uloc, 1, GL_FALSE, &(camera->model[0][0]) );
  }
  
  if( v_uloc != -1) {
    glUniformMatrix4fv(v_uloc, 1, GL_FALSE, &(camera->view[0][0]) );
  }
  

  if( _texture) {
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(texture_uloc, 0);
  }
  
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
			0,                  // attribute 0.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

  if( _color) {
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
			  1,                  // attribute 1
			  3,                  // size
			  GL_FLOAT,           // type
			  GL_FALSE,           // normalized?
			  0,                  // stride
			  (void*)0            // array buffer offset
			  );
  }


  if( _uv) {
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
			  2,                  // attribute 2
			  2,                  // size
			  GL_FLOAT,           // type
			  GL_FALSE,           // normalized?
			  0,                  // stride
			  (void*)0            // array buffer offset
			  );
  }

  if( _normal) {
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
			  3,                  // attribute 3
			  3,                  // size
			  GL_FLOAT,           // type
			  GL_FALSE,           // normalized?
			  0,                  // stride
			  (void*)0            // array buffer offset
			  );
  }

  glDrawArrays(GL_TRIANGLES, 0, num_vertices); // 3 indices starting at 0 -> 1 triangle
    
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);

}
