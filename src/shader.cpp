#include "shader.h"
#include "camera.h"


Shader::Shader()
{
  program = 0;
  _color = false;
  _normal = false;
  _uv = false;
  _tile = false;


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

void Shader::use_tile()
{
  _tile = true;
  glGenBuffers(1, &tilebuffer);
}



void Shader::use_texture()
{
  use_uv();
  _texture = true;
}


template<class T>
void Shader::move_to_buffer(GLuint buffer, std::vector<T> &v)
{
  std::vector<T> sorted;
  
  for( int pri=0; pri<8; pri++) {
    for( int ix=0; ix<v.size(); ix++) {
      if( priority[ix] == pri) {
	sorted.push_back( v[ix]);
      }
    }
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sorted.size() * sizeof(T), &sorted[0], GL_STATIC_DRAW);
}


void Shader::move_data_to_buffers()
{
  num_vertices = vertices.size();
  while( priority.size() < num_vertices) {
    priority.push_back( 0);
  }

  for( int ix=0; ix<num_pri; ix++) {
    pri_cnts[ix] = 0;
  }
  
  for( int ix=0; ix<num_vertices; ix++) {
    pri_cnts[ priority[ix]]++;
  }

  move_to_buffer( vertexbuffer, vertices);
  // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);

  if( _uv) {
    assert( uv.size() == num_vertices);
    move_to_buffer( uvbuffer, uv);
  }

  if( _color) {
    assert( color.size() == num_vertices);
    move_to_buffer( colorbuffer, color);
  }

  if( _normal) {
    assert( normal.size() == num_vertices);
    move_to_buffer( normalbuffer, normal);
  }
  
  if( _tile) {
    assert( tile.size() == num_vertices);
    move_to_buffer( tilebuffer, tile);
  }
  
  vertices.clear();
  uv.clear();
  color.clear();
  normal.clear();
  tile.clear();
  priority.clear();
}
 



void Shader::draw(int pri)
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

  if( _tile) {
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, tilebuffer);
    glVertexAttribPointer(
			  4,                  // attribute 3
			  2,                  // size
			  GL_FLOAT,	      // type
			  GL_FALSE,           // normalized?
			  0,                  // stride
			  (void*)0            // array buffer offset
			  );
  }

  int offset = 0;
  for( int ix=0; ix<pri; ix++) {
    offset += pri_cnts[ix];
  }
  
  glDrawArrays(GL_TRIANGLES, offset, pri_cnts[pri]); // 3 indices starting at 0 -> 1 triangle
    
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);

}
