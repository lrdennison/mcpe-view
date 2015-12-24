all: minedit

OPT = -I leveldb-mcpe/include -std=c++11

include leveldb-mcpe/build_config.mk

CFLAGS += -I. -I./include $(PLATFORM_CCFLAGS) $(OPT)
CXXFLAGS += --std=c++11 -DDLLX= -I. -I./include $(PLATFORM_CXXFLAGS) $(OPT)

LDFLAGS += $(PLATFORM_LDFLAGS)
LIBS += $(PLATFORM_LIBS) -lz

render.o: render.cpp Makefile
	gcc -ggdb ${CXXFLAGS} -c -o render.o render.cpp

main.o: main.cpp Makefile
	gcc -ggdb ${CXXFLAGS} -c -o main.o main.cpp

minedit: main.o Makefile
	$(CXX) -o minedit $(LDFLAGS) main.o leveldb-mcpe/libleveldb.a $(LIBS)



