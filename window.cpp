#include "window.h"
#include <iostream>
#include "shader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
using glm::vec3;
using glm::mat4;

using namespace std;


shader gpuProgram;
shader gpuLightSphere;
uint vbo[2];

uint vbo_vc;
uint vao_vc[1];



// Create a colored triangle

vector<float> vertices =  {0.00f,  0.75f, 0.0f, 1.0f,
                     0.75f, -0.75f, 0.0f, 1.0f,
                    -0.75f, -0.75f, 0.0f, 1.0f};

vector<float>  color ={ 0.0f, 1.0f, 0.0f, 1.0f,
                      0.0f, 0.0f, 1.0f, 1.0f,
                      1.0f, 0.0f, 0.0f, 1.0f};

typedef struct _vertexAttr{
   glm::vec4 vertex;
   glm::vec4 color;
   _vertexAttr(glm::vec4 inVertex, glm::vec4 inColor): vertex(inVertex), color(inColor){};

//   float vertex[4];
//   float color[4];
//   _vertexAttr(float *inVertex, float *inColor): vertex(inVertex), color(inColor){};

} VertexAttr;



uint VAO[2];

int loc_attribute_Position;
int loc_attribute_Color;


Window::~Window()
{
  makeCurrent();
  teardownGL();
}

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

void Window::initializeGL()
{

    VertexAttr temp1(glm::vec4(0.00f,  0.75f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

//    VertexAttr temp2[3]{{glm::vec4( 0.00f,  0.75f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
//                        {glm::vec4( 0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
//                        {glm::vec4(-0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }};

//    VertexAttr temp2[3]{{glm::vec4( 0.00f,  0.75f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
//                        {glm::vec4( 0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
//                        {glm::vec4(-0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }};

    vector<VertexAttr> temp2{{glm::vec4( 0.00f,  0.75f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
                        {glm::vec4( 0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}/*,
                        {glm::vec4(-0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }*/};

    temp2.push_back(VertexAttr(glm::vec4(-0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));


    cout << glm::to_string(temp2[2].vertex) << glm::to_string(temp2[2].color) << endl;
    cout << "tamanho da struquite: " << sizeof(VertexAttr) << endl << "tamanho da variavel: " << sizeof(temp2) << endl;
    cout << "Deslocamento de apontador - vertice: " << offsetof(VertexAttr, vertex) << endl;
    cout << "Deslocamento de apontador - color  : " << offsetof(VertexAttr, color) << endl;



    initializeOpenGLFunctions();
    //This call a class that will load a program and attach shaders on it, as well as making possible taking information on its variables
    gpuProgram.loadProgram("./hello.vert","./hello.frag");
    //gpuProgram.useProgram();


    //Print information of the variables/their address that the GPU can send to us
    //gpuProgram.programVarInfo();

    //generating two indexes for two memory spaces in GPU
    glGenBuffers(2,vbo);

    //Sending array to GPU memory 1st slot alocated and getting an index to it
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //making this memory the current active
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(),vertices.data(),GL_STATIC_DRAW); //send the data to this memory

    //Sending array to GPU memory 2st slot alocated and getting an index to it
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*color.size(),color.data(),GL_STATIC_DRAW);

    //deactivating whatever buffer is on and receiving data
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /////////////////////////////////////////////////////////////////////
    glGenBuffers(1,&vbo_vc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vc);

    glBufferData(GL_ARRAY_BUFFER, temp2.size()*sizeof(VertexAttr),temp2.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, vao_vc);


    glBindVertexArray(vao_vc[0]);
    loc_attribute_Position =glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Position");
    loc_attribute_Color = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Color");

    glEnableVertexAttribArray(loc_attribute_Position);
    glEnableVertexAttribArray(loc_attribute_Color);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vc);
    glVertexAttribPointer(loc_attribute_Position, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride? =*/ sizeof(VertexAttr), (void*)offsetof(VertexAttr, vertex));
    glVertexAttribPointer(loc_attribute_Color, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride?=*/ sizeof(VertexAttr), (void*)offsetof(VertexAttr, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //turning off the VAO
    glBindVertexArray(0);

    /////////////////////////////////////////
    //getting the location of the two variables in the shaders
    loc_attribute_Position =glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Position");
    loc_attribute_Color = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Color");


   //generating one Vertex Array Object  (VAO)
    glGenVertexArrays(1, VAO);

    //Activating VAO
    glBindVertexArray(VAO[0]);
   // glBindVertexArray(vao_vc[0]);
    //Setting the relationship between what is stored in the buffers to the variables in shader
    //In this case, vbo[0] is the list of vertex and vbo[1] is the list of colors associated with the vertex

    //Getting the location of each variable on shader
    glEnableVertexAttribArray(loc_attribute_Position);
    glEnableVertexAttribArray(loc_attribute_Color);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(loc_attribute_Position, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride? =*/ 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(loc_attribute_Color, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride?=*/ 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //turning off the VAO
    glBindVertexArray(0);


}

void Window::resizeGL(int width, int height)
{

  // Currently we are not handling width/height changes
  //(void)width;
  //(void)height;
    cout << "(width, height): (" << width << " , " << height << ")" << endl;
//if(width < height)
//    glViewport(0, 0, width, width);
//else
//    glViewport(0, 0,  height, height);

}


void Window::paintGL()
{    

    float bg[] = {0.0f,0.0f,0.0f,1.0};

    //Clean screen and setting the background with the color bg
    glClearBufferfv(GL_COLOR, 0, bg);

    //Activating program/VAO
    gpuProgram.useProgram();
   // glBindVertexArray(VAO[0]);
    glBindVertexArray(vao_vc[0]);

    glDrawArrays(GL_LINE_STRIP, 0, 2);

    //Deactivating program/VAO (not necessary in this case, since there is only ONE program and ONE VAO)
    glBindVertexArray(0);
    gpuProgram.release();

    //Activating program/VAO
//    gpuProgram.useProgram();
//    glBindVertexArray(VAO[0]);

//    glDrawArrays(GL_TRIANGLES, 0, 3);

//    //Deactivating program/VAO (not necessary in this case, since there is only ONE program and ONE VAO)
//    glBindVertexArray(0);
//    gpuProgram.release();



    //replace the previous draw with the new draw, consisting in what background with the content of what the contents of glDrawArrays
    update();


}

void Window::teardownGL()
{
    gpuProgram.release();
    exit(0);
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void Window::printContextInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

//  std::cout << glVersion.toStdString()  << std::endl;
  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
