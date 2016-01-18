bin_PROGRAMS = minedit

minedit_SOURCES = main.cpp
minedit_SOURCES += database.h
minedit_SOURCES += shader_program.cpp shader_program.h
minedit_SOURCES += shader.cpp shader.h
minedit_SOURCES += color_shader.cpp color_shader.h
minedit_SOURCES += texture_shader.cpp texture_shader.h
minedit_SOURCES += block_shader.cpp block_shader.h
minedit_SOURCES += gd_shader.cpp gd_shader.h
minedit_SOURCES += basic_model.cpp basic_model.h
minedit_SOURCES += cube.cpp cube.h
minedit_SOURCES += block.cpp block.h
minedit_SOURCES += viewer.cpp viewer.h
minedit_SOURCES += texture_atlas.cpp texture_atlas.h
minedit_SOURCES += texture_factory.cpp texture_factory.h
minedit_SOURCES += camera.cpp camera.h


minedit_SOURCES += block_info.cpp block_info.h
minedit_SOURCES += packed_block.cpp packed_block.h
minedit_SOURCES += terrain.cpp terrain.h
minedit_SOURCES += terrain_chunk.cpp terrain_chunk.h

minedit_SOURCES += drawable.h
minedit_SOURCES += my_drawable.cpp
minedit_SOURCES += grid_drawable.cpp
minedit_SOURCES += hud_drawable.cpp

CXXFLAGS = -ggdb -std=c++11 -DDLLX=  -I ../leveldb-mcpe/include

AM_LDFLAGS =  -ggdb

LDADD = 
LDADD += -L../leveldb-mcpe -L../common
LDADD += -lglfw -lGL -lGLEW -lglut -lz -lsnappy -lpthread -lgd -llog4cxx ../leveldb-mcpe/libleveldb.a ../common/libcommon.a
