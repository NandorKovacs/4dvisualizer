#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <GL/glew.h>

#include <string>

GLuint create_shader_program(std::string vshader_file, std::string fshader_file);

#endif  // SHADER_LOADER_H
