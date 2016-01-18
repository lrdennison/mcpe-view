#include "common.h"
#include "texture_atlas.h"
#include "camera.h"
#include "viewer.h"
#include "database.h"
#include "terrain.h"

using namespace std;


uint64_t gettid()
{
  return 0;
}


TextureAtlas *terrain_atlas;
Camera *camera;
std::string root_dir;

static LoggerPtr logger(Logger::getLogger("MyApp"));

int main(int arg, char **argv)
{
  root_dir = "/home/larry/mcpe-view/";
  
  //BasicConfigurator::configure();
  PropertyConfigurator::configure(root_dir + "src/logging.conf");
  
  LOG4CXX_INFO(logger, "Entering application.")

  terrain_atlas = new TextureAtlas();
  terrain_atlas->load();

  Database::open("../world/db");

  Terrain::scan_db();

  camera = new Camera();
  
  Viewer viewer;
  MyDrawable *drawable = new MyDrawable();
  viewer.add( drawable); 
  viewer.add( &Terrain::Inst() ); 
  viewer.add( new GridDrawable());
  viewer.add( new HUDDrawable());

  try {
    viewer.init();
    viewer.run();
  }
  catch( Exc e) {
    printf("Caught exception: %s\n", e.msg.c_str());
  }

  return 0;
}
