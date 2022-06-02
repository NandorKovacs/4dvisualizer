all: cube

UNAME := $(shell uname)

CPPFLAGS := -Wall -I$(HOME)/opt/include --std=c++20 -g
LDFLAGS :=  -L$(HOME)/opt/lib

ifeq (, $(shell which clang++))
CC := g++
CXX := g++
else
CC := clang++
CXX := clang++
endif

ifeq ($(UNAME), Darwin)
LDLIBS_EXTRA := -lglfw3 \
	-framework OpenGL \
	-framework CoreFoundation \
	-framework IOKit \
	-framework AppKit
else
LDLIBS_EXTRA := -lglfw -lGL
endif

LDLIBS := -lGLEW $(LDLIBS_EXTRA) 

.PHONY: clean all

clean:
	rm -f *.o cube

lib/lib.a:
	$(MAKE) -C lib

cube: cube.o renderer.o shader_loader.o window_size.o lib/lib.a

# to regenerate the following part: g++ -MM *.cc >> Makefile


camera_manager.o: camera_manager.cc camera_manager.h
cube.o: cube.cc camera_manager.h input_handler.h lib/errors.h renderer.h \
 window_size.h
input_handler.o: input_handler.cc input_handler.h camera_manager.h \
 lib/errors.h
renderer.o: renderer.cc renderer.h camera_manager.h lib/errors.h \
 shader_loader.h
shader_loader.o: shader_loader.cc shader_loader.h lib/errors.h
window_size.o: window_size.cc window_size.h lib/errors.h
