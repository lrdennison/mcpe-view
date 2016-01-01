#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"

// Object storage representations

struct DbKey9 {
  int32_t x;
  int32_t z;
  unsigned char type;

  std::string toStr()
  {
    std::string s( reinterpret_cast<char const*>(this), 9);
    return s;
  }
  
};



class Database : public Singleton<Database>
{
public:
  leveldb::DB* db;

  Database()
  {
    db = NULL;
    zcomp = new leveldb::ZlibCompressor();
  }

  static void open( std::string path)
  {
    Inst()._open( path);
  }
  

  static void get( std::string &value, int x, int z, char type)
  {
    Inst()._get( value, x, z, type);
  }
  
  
protected:
  leveldb::Compressor *zcomp;
  leveldb::Options options;

private:
  void _open( std::string path)
  {
    options.create_if_missing = false;
    options.paranoid_checks = true;
    options.compressors[0] = zcomp;
    // options.compressors[1] = leveldb::SnappyCompressor();

    leveldb::Status status = leveldb::DB::Open(options, "/home/larry/world/db", &db);
    assert(status.ok());
  }

  void _get(std::string &value, int x, int z, char type)
  {
    DbKey9 key;
    key.x = x;
    key.z = z;
    key.type = type;

    leveldb::Status s = db->Get(leveldb::ReadOptions(), key.toStr(), &value);
  }

};


struct DbTerrain {
  unsigned char block[32768];
  unsigned char dv[16384];
  unsigned char skylight[16384];
  unsigned char block_light[16384];
  unsigned char dirty_col[256];
  uint32_t grass_color[256];
};



#endif

