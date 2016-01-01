#include "color_shader.h"


ColorShader::ColorShader()
{
  program = ShaderProgramFactory::get("color");
  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);

  mvp_id = glGetUniformLocation(program->id, "MVP");
}


void ColorShader::move_data_to_buffers()
{
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
}
 
void ColorShader::draw()
{
  glUseProgram(program->id);
  glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &(camera->mvp[0][0]) );

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

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(
			1,                  // attribute 0.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 3 indices starting at 0 -> 1 triangle
    
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}
