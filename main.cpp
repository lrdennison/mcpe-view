#include <iostream>
#include <stdio.h>

#include "leveldb/db.h"
#include "leveldb/zlib_compressor.h"

using namespace std;

uint64_t gettid()
{
  return 0;
}


struct Key8 {
  int32_t x;
  int32_t z;
  unsigned char type;
};

struct Terrain {
  unsigned char block[32768];
  unsigned char dv[16384];
  unsigned char skylight[16384];
  unsigned char block_light[16384];
};

int main(int arg, char **argv)
{
  leveldb::Compressor *zcomp;
  zcomp = new leveldb::ZlibCompressor();

  printf("Hello world\n");

  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = false;
  options.paranoid_checks = true;
  options.compressors[0] = zcomp;
  // options.compressors[1] = leveldb::SnappyCompressor();

  leveldb::Status status = leveldb::DB::Open(options, "data/db", &db);
  assert(status.ok());

  leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

  cout << "Checking iterator... ";
  assert(it->status().ok());  // Check for any errors found during the scan
  cout << "ok" << endl;

  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    string key, value;

    key = it->key().ToString();
    value = it->value().ToString();

    int len = key.length();
    int vlen = value.length();

    if( len == 9) {

      leveldb::Slice slice;
      slice = it->key();
      Key8 key;
      memcpy( (char *)&key, it->key().data(), 9);

      const char *data = it->value().data();

      // Terrain data
      if( key.type == '0') {

	Terrain *t = (Terrain *)data;
	int gcnt = 0;
	for( int ix=0; ix<32768; ix++) {
	  unsigned char dv = t->dv[ix>>1];
	  if( ix & 0x1) {
	    dv = dv >> 4;
	  }
	  dv = dv & 0xF;

	  if( t->block[ix] == 0xAB /* && dv==2 */ ) {
	    gcnt++;
	  }
	}

	if( gcnt > 0) {
	  cout << key.x << ":" << key.z << "  ";
	  cout << "Gold:" << gcnt << endl;
	}

      }
    }

    // cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
  }
  assert(it->status().ok());  // Check for any errors found during the scan
  delete it;

  return 0;
}
