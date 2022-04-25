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

cube.o: cube.cc lib/errors.h window_size.h renderer.h
renderer.o: renderer.cc renderer.h shader_loader.h lib/errors.h
shader_loader.o: shader_loader.cc shader_loader.h lib/errors.h
window_size.o: window_size.cc window_size.h lib/errors.h
