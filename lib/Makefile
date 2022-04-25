all: lib.a

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
LDLIBS_EXTRA :=  \
	-framework OpenGL \
	-framework CoreFoundation \
	-framework IOKit \
	-framework AppKit
else
LDLIBS_EXTRA := -lGL
endif

LDLIBS := -lGLEW $(LDLIBS_EXTRA)

.PHONY: clean all

clean:
	rm -f *.o lib.a

lib.a: errors.o
	ar rcs $@ $^
	
errors.o: errors.cc errors.h
