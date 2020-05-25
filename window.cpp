#include "window.h"
#include <iostream>
#include "shader.h"
#include "openGLAdditionals.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "indexbuffer.h"
#include "vertexbuffer.h"
#include "vertexarray.h"
using glm::vec3;
using glm::mat4;

using namespace std;


shader gpuProgram;
shader gpuLightSphere;
//uint vbo[2];

uint vbo_vc;
uint vbo_temp3;

VertexBuffer *vbo = new VertexBuffer[3];
VertexArray *vao = new VertexArray[1];



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
                         {glm::vec4( .75f,  .75f,   0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
                         {glm::vec4( 0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
                         {glm::vec4(-0.75f, -0.75f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)}
                        };
//    temp2.push_back(VertexAttr(glm::vec4(0.75f, 0.75f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
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


    gpuProgram.loadProgram("./hello.vert","./hello.frag");
    loc_attribute_Position =glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Position");
    loc_attribute_Color = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Color");
    //gpuProgram.useProgram();    
    //gpuProgram.programVarInfo();
    vbo[2].updateBufferData(temp2.data(),temp2.size()*sizeof(VertexAttr));

    vao->push<float>(loc_attribute_Position, 4);
    vao->push<float>(loc_attribute_Color, 4);
    vao->addBuffer(vbo[2]);

    _check_gl_error(__FILE__,__LINE__);

//    printContextInformation();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);


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

    gpuProgram.useProgram();
    vao->bind();
    unsigned int pidx[] = {0,1,2,3,4,5};
    IndexBuffer idx((pidx), 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    vao->unbind();
    gpuProgram.release();
//

//    //Activating program/VAO
//    gpuProgram.useProgram();
//    glBindVertexArray(VAO[0]);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);
//    gpuProgram.release();

//    gpuProgram.useProgram();
//    glBindVertexArray(VAO[1]);
//    unsigned int pidx[] = {0,1,2};
//    IndexBuffer idx((pidx), 3);
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
//    glBindVertexArray(0);
//    gpuProgram.release();

    update();


}

void Window::teardownGL()
{
    glUseProgram(0);
    gpuProgram.release();
    delete[] vbo;
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
