#include "block_shader.h"
#include "texture_factory.h"

BlockShader::BlockShader()
{
  texture = TextureFactory::get("terrain");
}
