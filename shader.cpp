#include "shader.h"
#include <vector>

shader::shader()
{
}

shader::shader(const char *vert_path, const char *frag_path)
{
    program = glCreateProgram();
    isProgramLinked = false;

    bool isVertexShaderLoaded   =  loadVertexShader(vert_path);
    bool isFragmentShaderLoaded = loadFragmentShader(frag_path);

    if (!(isVertexShaderLoaded && isFragmentShaderLoaded))
        return;

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    isProgramLinked = linkProgram();

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

shader::shader(std::string vert_path, std::string frag_path)
{
    const char *vert_pathSrc = vert_path.c_str();
    const char *frag_pathSrc = frag_path.c_str();

    program = glCreateProgram();
    isProgramLinked = false;

    bool isVertexShaderLoaded   =  loadVertexShader(vert_pathSrc);
    bool isFragmentShaderLoaded = loadFragmentShader(frag_pathSrc);

    if (!(isVertexShaderLoaded && isFragmentShaderLoaded))
        return;

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    isProgramLinked = linkProgram();

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

}

void shader::loadProgram(const char *vert_path, const char *frag_path)
{
    program = glCreateProgram();
    isProgramLinked = false;

    bool isVertexShaderLoaded   =  loadVertexShader(vert_path);
    bool isFragmentShaderLoaded = loadFragmentShader(frag_path);

    if (!(isVertexShaderLoaded && isFragmentShaderLoaded))
        return;

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    isProgramLinked = linkProgram();
    useProgram();

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

}

shader::~shader()
{
    if(program > 0)
            glDeleteProgram(program);
    glUseProgram(0);
    delete this;

}

unsigned int shader::getProgramID()
{
    return program;
}



bool shader::loadVertexShader(const char *vert_path)
{

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertShaderStr = readText(vert_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    GLint result = GL_FALSE;
    int logLength;

    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> vertShaderError(logLength);
        glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
        std::cout << "Vertex shader compilation failed!" << std::endl;
        std::cout << &vertShaderError[0] << std::endl;
    }
    return result;

}

bool shader::loadFragmentShader(const char* frag_path)
{
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragShaderStr = readText(frag_path);
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;


    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE)
    {
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector <char> fragShaderError(logLength);
        glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
        std::cerr << "Fragment shader compilation failed!" << std::endl;
        std::cerr << &fragShaderError[0] << std::endl;
    }
    return result;
}

bool shader::linkProgram()
{
    GLint result = GL_FALSE;
    int logLength;

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> programError( (logLength > 1) ? logLength : 1 );
        glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
        std::cerr << &programError[0] << std::endl;
    }

    return result;
}

bool shader::useProgram()
{
    if (!program)
    {
        std::cerr << "The program doesn't exist" << std::endl;
        return false;
    }
    if (!isProgramLinked)
    {
         std::cerr << "Program not linked" << std::endl;
        return false;
    }
    glUseProgram(program);
    return true;
}

void shader::programVarInfo()
{
    if (!program)
    {
        std::cerr << "The program doesn't exist" << std::endl;
        return;
    }
    GLint maxLength, nAttribs;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    GLchar * name = (GLchar *) malloc( maxLength );

    GLint written, size, location;
    GLenum type;
    std::cout <<"Input Variables" << std::endl;
   // std::cout <<"------------------------------------------------" << std::endl;
    std::cout << " Index | Name"<< std::endl;
  //  std::cout << "------------------------------------------------" << std::endl;
    for( int i = 0; i < nAttribs; i++)
    {
        glGetActiveAttrib(program, i, maxLength, &written, &size, &type, name );
        location = glGetAttribLocation(program, name);
        std::cout <<" " << location <<" | " << name << std::endl;
    }
    free(name);

    GLint nUniforms, maxLen;
    glGetProgramiv( program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
    &maxLen);
    glGetProgramiv( program, GL_ACTIVE_UNIFORMS,
    &nUniforms);

    std::cout <<"------------------------------------------------" << std::endl;
    std::cout <<"Uniform Variables" << std::endl;

    std::cout << " Index | Name" << std::endl;
    //std::cout << "------------------------------------------------\n" << std::endl;
    for( int i = 0; i < nUniforms; ++i )
    {
    glGetActiveUniform( program, i, maxLen, &written,
    &size, &type, name );
    location = glGetUniformLocation(program, name);
    std::cout <<" " << location <<" | " << name << std::endl;
    }
    std::cout <<"------------------------------------------------" << std::endl;
}

void shader::release()
{
    glUseProgram(0);
}

std::string shader::readText(const char *filePath)
{
    std::string content;
       std::ifstream fileStream(filePath, std::ios::in);

       if(!fileStream.is_open()) {
           std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
           return "";
       }

       std::string line = "";
       while(!fileStream.eof()) {
           std::getline(fileStream, line);
           content.append(line + "\n");
       }
       fileStream.close();
       return content;

}
