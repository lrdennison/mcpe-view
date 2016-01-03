#ifndef COMMON_H
#define COMMON_H

// Include standard headers

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <gd.h>

#include "leveldb/db.h"
#include "leveldb/zlib_compressor.h"

#include <log4cxx/logger.h>
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
#include <log4cxx/xml/domconfigurator.h>
#include "log4cxx/propertyconfigurator.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glut.h>

// From the utility
#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/texture.hpp>

#include "singleton.h"

typedef glm::vec4 Vertex;
typedef glm::vec2 UV;
typedef glm::vec3 Normal;

typedef std::vector<Vertex> Vertices;
typedef std::vector<UV> UVs;
typedef std::vector<Normal> Normals;


class Exc : public std::exception {
 public:
  std::string msg;

  Exc( std::string s) {
    this->msg = s;
  }
  
};

extern class TextureAtlas *terrain_atlas;
extern class Camera *camera;


#endif
