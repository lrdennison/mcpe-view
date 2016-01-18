#include "block_shader.h"
#include "texture_factory.h"

BlockShader::BlockShader()
{
  use_program("light");
  use_normal();
  use_tile();
  texture = TextureFactory::get("terrain");
}
