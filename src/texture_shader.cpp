#include "texture_shader.h"


TextureShader::TextureShader()
{
  use_texture();
  //use_normal();
}


/*
void TextureShader::draw()
{
  glUseProgram(program->id);
  glUniformMatrix4fv(mvp_uloc, 1, GL_FALSE, &(camera->mvp[0][0]) );

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(texture_uloc, 0);

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

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer(
			2,                  // attribute 0.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

  glDrawArrays(GL_TRIANGLES, 0, num_vertices); // 3 indices starting at 0 -> 1 triangle
    
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(2);

}
*/
