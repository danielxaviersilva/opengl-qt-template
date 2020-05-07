#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <gl.h>
//#include<GL/glew.h>
#include <vector>


class shader
{
    unsigned int vertShader, fragShader, program;
    bool isProgramLinked;
public:
    shader();
    shader(const char * vert_path, const char * frag_path);
    shader(std::string vert_path, std::string frag_path);

    void loadProgram(const char * vert_path, const char * frag_path);

    ~shader();

    unsigned int getProgramID();

    uint LoadShader(const char *vertex_path, const char *fragment_path);

    bool loadVertexShader(const char *vert_path);
    bool loadFragmentShader(const char *frag_path);

    bool linkProgram();

    bool useProgram();
    void programVarInfo();

    void release();

    //aux functions
    std::string readText(const char *filePath);
};

#endif // SHADER_H

