#include "window.h"
#include <iostream>
#include "shader.h"
#include "openGLAdditionals.h"


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
uint vbo_temp3;



// Create a colored triangle

vector<float> vertices =  { .75f,  .75f,   0.0f, 1.0f,
                            0.75f, -0.75f, 0.0f, 1.0f,
                           -0.75f, -0.75f, 0.0f, 1.0f};

vector<float>  color ={ 1.0f, 1.0f, 1.0f, 0.5f,
                        1.0f, 1.0f, 1.0f, 0.5f,
                        1.0f, 1.0f, 1.0f, 0.5f};

typedef struct _vertexAttr{
   glm::vec4 vertex;
   glm::vec4 color;
   _vertexAttr(glm::vec4 inVertex, glm::vec4 inColor): vertex(inVertex), color(inColor){};

//   float vertex[4];
//   float color[4];
//   _vertexAttr(float *inVertex, float *inColor): vertex(inVertex), color(inColor){};

} VertexAttr;
vector<VertexAttr> temp2{{glm::vec4( 0.00f,  0.75f, 0.5f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
                         {glm::vec4(-0.75f, -0.75f, 0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
                         {glm::vec4( 0.75f, -0.75f, -0.5f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
                        };

uint VAO[3];

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
      initializeOpenGLFunctions();
//    temp2.push_back(VertexAttr(glm::vec4(0.75f, 0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

    //This call a class that will load a program and attach shaders on it, as well as making possible taking information on its variables
    gpuProgram.loadProgram("./hello.vert","./hello.frag");
    //gpuProgram.useProgram();    
    //gpuProgram.programVarInfo();


    /*TWO BUFFERS WITH VERTICES AND COLOR ALLOCATION*/
    //generating two indexes for two memory spaces in GPU
    glGenBuffers(2,vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(),vertices.data(),GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*color.size(),color.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);    
    //getting the location of the two variables in the shaders
    loc_attribute_Position =glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Position");
    loc_attribute_Color = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Color");

    glGenVertexArrays(3, VAO);
    glBindVertexArray(VAO[0]);

    glEnableVertexAttribArray(loc_attribute_Position);
    glEnableVertexAttribArray(loc_attribute_Color);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(loc_attribute_Position, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride? =*/ 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(loc_attribute_Color, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride?=*/ 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    /*////////////RENDERING VERTICES/COLORS THAT ARE ATTRIBUTES OF A STRUCT \\\\\\\\\*/
    glGenBuffers(1,&vbo_vc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vc);
    glBufferData(GL_ARRAY_BUFFER, temp2.size()*sizeof(VertexAttr),temp2.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /*EOA*/

    glBindVertexArray(VAO[1]);
    loc_attribute_Position =glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Position");
    loc_attribute_Color = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Color");
    glEnableVertexAttribArray(loc_attribute_Position);
    glEnableVertexAttribArray(loc_attribute_Color);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vc);
    glVertexAttribPointer(loc_attribute_Position, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride? =*/ sizeof(VertexAttr), (void*)offsetof(VertexAttr, vertex));
    glVertexAttribPointer(loc_attribute_Color, 4, GL_FLOAT, /*normalize? =*/ GL_FALSE, /*stride?=*/ sizeof(VertexAttr), (void*)offsetof(VertexAttr, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_CULL_FACE);
    _check_gl_error(__FILE__,__LINE__);

//    printContextInformation();


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



//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*EXAMPLE OF - DRAWING WITH glDrawArrays*/
    float bg[] = {0.0f,0.0f,0.0f,1.0};
    glClear( GL_DEPTH_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, bg);
//

//    //Activating program/VAO
    gpuProgram.useProgram();
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    gpuProgram.release();


    gpuProgram.useProgram();
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    gpuProgram.release();

////    GLushort pidx[] = {0,1,2};

////    unsigned int vboID;
////    glGenBuffers(1, &vboID);
////    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
////    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pidx), pidx, GL_STATIC_DRAW);
////    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
////    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);
////    glDeleteBuffers(1, &vboID);

//    glClearBufferfv(GL_COLOR, 0, bg);

//     gpuProgram.useProgram();
//     glBindVertexArray(VAO[2]);
//     glDrawArrays(GL_TRIANGLES, 0, 6);
//    glBindVertexArray(0);
//    gpuProgram.release();




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

//int i = 0;
//void Window::voidmouseMoveEvent(QMouseEvent *ev)
//{
//    std::cout << "Alguem arrastou o mouse? " << i++ << std::endl;
//}

//void Window::mousePressEvent(QMouseEvent *ev)
//{
//    std::cout << "Alguem mexeu no mouse? " << i++ << std::endl;
//}

//void Window::moveEvent(QMoveEvent *ev)
//{
//    std::cout << "Alguem arrastou o mouse? " << i++ << std::endl;
//}
