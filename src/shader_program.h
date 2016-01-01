#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "common.h"

class ShaderProgram {
public:
  GLuint id;
  
  std::vector<GLuint> obj_ids;
    
  ShaderProgram();
  void load_from_source(std::string v_path, std::string f_path);

  std::string read_code(std::string fn);
    
  void compile(std::string path, GLuint kind);
  void link();

private:
  static LoggerPtr logger;
};


class ShaderProgramFactory : public Singleton<ShaderProgramFactory>
{
 public:
  ShaderProgram *_get(std::string name);
  static ShaderProgram *get(std::string name)
  {
    return ShaderProgramFactory::Inst()._get(name);
  }

 private:
  static LoggerPtr logger;

  std::map<std::string, ShaderProgram *> m;
};

#endif
