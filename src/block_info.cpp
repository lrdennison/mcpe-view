#include "block_info.h"
#include "texture_atlas.h"
#include "cube.h"

BlockInfo raw_block_info[] = {
  {0x00, "air", false, false},
  {0x01, "stone", true, true},
  {0x02, "grass", true, false},
  {0x03, "dirt", true, false},

  {0x04, "cobblestone", true, false},
  {0x05, "wooden_planks", true, false},
  {0x06, "sapling", false, false},
  {0x07, "bedrock", true, false},

  {0x08, "flowing_water", true, false},
  {0x09, "still_water", true, false},
  {0x0A, "lava", false, false},
  {0x0B, "stationary_lava", false, false},

  {0x0C, "sand", true, false},
  {0x0D, "gravel", true, false},
  {0x0E, "gold_ore", true, false},
  {0x0F, "iron_ore", true, false},

  {0x10, "coal_ore", true, false},
  {0x11, "log", true, false},	// aka wood
  {0x12, "leaves", false, false},
  {0x13, "sponge", true, false},

  //{0x14, "glass", false, false},
  {0x15, "lapis_lazuli_ore", true, false},
  {0x16, "lapis_lazuli_block", false, false},

  {0x18, "sandstone", true, false},
  
  {0x49, "redstone_ore", true, false},
  {0x4A, "redstone_block", true, false},

  {0x56, "pumpkin", true, false},
  {0x3C, "farmland", true, false},

  {52, "mob_spawner", true, false},
  {58, "crafting_table", true, false},
  {103, "melon", true, false},
};

LoggerPtr BlockInfoBase::logger(Logger::getLogger("BlockInfoBase"));

BlockInfoBase::BlockInfoBase()
{
  for(int ix=0; ix<256; ix++) {
    m[ix] = 0;
  }

  for( auto b: raw_block_info) {
    BlockInfo *bip = new BlockInfo();
    *bip = b;
    m[b.id] = bip;
  }

  for( auto bip: m) {
    if( !bip)
      continue;

    // Default render function
    bip->render_func = get_block_renderer( bip->name);

    
    if( terrain_atlas->find(bip->name) != terrain_atlas->end()) {
      bip->atlas_item = (*terrain_atlas)[bip->name];
      LOG4CXX_INFO( logger, "Found " << bip->name);
    }
    else {
      LOG4CXX_WARN( logger, "Couldn't find " << bip->name);
    }

    std::string s = bip->name + "2";
    if( terrain_atlas->find(s) != terrain_atlas->end()) {
      bip->atlas_item2 = (*terrain_atlas)[bip->name];
      LOG4CXX_INFO( logger, "Found " << s);
    }

    
  }
}



BlockInfo *BlockInfoBase::get( int id)
{
  return Inst()._get( id);
}

BlockInfo *BlockInfoBase::_get( int id)
{
  return m[id];
}
