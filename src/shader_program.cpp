#include <iostream>
#include <fstream>
#include <algorithm>

#include "shader_program.h"

using namespace std;

LoggerPtr ShaderProgram::logger(Logger::getLogger("ShaderProgram"));

ShaderProgram::ShaderProgram()
{
}

void ShaderProgram::load_from_source(string v_path, string f_path)
{
  compile( v_path, GL_VERTEX_SHADER);
  compile( f_path, GL_FRAGMENT_SHADER);
  link();
}


std::string ShaderProgram::read_code(string fn)
{
  // Read the shader code from the file
  string code;
  string rfn = root_dir + "shaders/" + fn;
  ifstream s(rfn, std::ios::in);
  if(s.is_open()){
    string line = "";
    while(getline(s, line))
      code += "\n" + line;
    s.close();
  }
  else {
    throw Exc( std::string("Impossible to open shader ") + fn);
  }
  return code;
}


void ShaderProgram::compile(string path, GLuint kind)
{
  LOG4CXX_INFO( logger, "Compiling " << path);
  
  string code = read_code( path);
  GLuint id = glCreateShader( kind);
  
  char const *s = code.c_str();
  glShaderSource(id, 1, &s , NULL);
  glCompileShader(id);


  GLint result = GL_FALSE;
  int len;
  char msg[4096];

  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
  if ( len > 0 ) {
    glGetShaderInfoLog(id, len, NULL, msg);
    LOG4CXX_ERROR(logger, msg);
  }

  obj_ids.push_back( id);

}


void ShaderProgram::link()
{
  LOG4CXX_INFO(logger, "Linking program");
  id = glCreateProgram();
  for( auto obj_id : obj_ids) {
    glAttachShader(id, obj_id);
  }
  glLinkProgram(id);

  // Check the program
  GLint result = GL_FALSE;
  int len;
  char msg[4096];

  glGetProgramiv(id, GL_LINK_STATUS, &result);
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
  if ( len > 0 ){
    glGetProgramInfoLog(id, len, NULL, msg);
    printf("%s\n", msg);
  }

	
  for( auto obj_id : obj_ids) {
    glDetachShader(id, obj_id);
    glDeleteShader(obj_id);
  }
  obj_ids.clear();

}


LoggerPtr ShaderProgramFactory::logger(Logger::getLogger("ShaderProgramFactory"));


ShaderProgram *ShaderProgramFactory::_get(std::string name)
{
  LOG4CXX_DEBUG( logger, "Request for " << name);
  ShaderProgram *prog;

  if( m.find( name) == m.end()) {

    LOG4CXX_INFO( logger, "Loading from source for " << name);

    prog = new ShaderProgram();
    
    if( name == "color") {
      prog->load_from_source( "color.vs", "color.fs" );
    }
    if( name == "texture") {
      prog->load_from_source( "texture.vs", "texture.fs");
    }
    
    if( name == "light") {
      prog->load_from_source( "light.vs", "light.fs");
    }
    
    LOG4CXX_INFO( logger, "Finished");
    m[name] = prog;
  }

  return m[name];
}

