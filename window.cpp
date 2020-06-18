#include "window.h"
#include <iostream>

#include "openGLAdditionals.h"


#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>
#include "./Utilities/shader.h"
#include "./Utilities/indexbuffer.h"
#include "./Utilities/vertexbuffer.h"
#include "./Utilities/vertexarray.h"
#include "./Utilities/camera.h"

#include "./Primitives/sphere.h"
#include "./Primitives/cube.h"
#include "./Primitives/cilinder.h"


#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace std;


shader gpuProgram;
shader gpuLightSphere;
//uint vbo[2];

uint vbo_vc;
uint vbo_temp3;

VertexBuffer *vbo = new VertexBuffer[3];
VertexArray *vao = new VertexArray[1];
Camera cam;
Cube cube1(0.25,glm::vec3(0.5,0.5,0.0), glm::vec3(1.0,1.0,0.0));
Cilinder cilinder1(30, 30,0.1f,
                  0.5f,glm::vec3(0.0),glm::vec3(1.0, 0.0, 0.0));



// Create a colored triangle
Sphere sphere1(100,100,0.5, glm::vec3(0.0f,0.0f, 0.0f));
Sphere sphere2(100,100,0.125, glm::vec3(-0.75f,0.750f, 0.0f));
Sphere sphere3(500,500,0.5, glm::vec3(0.0f,0.0f, 0.0f));


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
vector<VertexAttr> temp2{{glm::vec4( 0.00f,  0.0f,   0.f,  1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.7f)},
                         {glm::vec4( 0.2f,  0.0f,    0.f,  1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.7f)},
                         {glm::vec4( 0.0f,  0.2f,    0.f,  1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.7f)},
                         {glm::vec4( 0.00f,  0.0f,   0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 0.7f)},
                         {glm::vec4( 0.2f,  0.0f,    0.5f, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, 0.7f)},
                         {glm::vec4( 0.0f,  0.2f,    0.5f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 0.7f)},
                         {glm::vec4( 0.00f,  0.0f,   0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
                         {glm::vec4( 0.0f,  0.75f,   0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
                         {glm::vec4(0.75f,  0.0f,    0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
                         {glm::vec4( .75f,  .75f,    0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
                         {glm::vec4( 0.75f, -0.75f,  0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
                         {glm::vec4(-0.75f, -0.75f,  0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)}
                        };


//vector<VertexAttr> temp2{{glm::vec4( 0.00f,  -1.0f,   0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
//                         {glm::vec4(0.0f,  1.0f,    0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
//                         {glm::vec4(1.0f,  0.0f,   0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
//                         {glm::vec4(-1.00f,  0.0f,   0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
//                         {glm::vec4( 0.0f,  0.75f,   0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
//                         {glm::vec4(0.75f,  0.0f,    0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
//                         {glm::vec4( .75f,  .75f,    0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
//                         {glm::vec4( 0.75f, -0.75f,  0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
//                         {glm::vec4(-0.75f, -0.75f,  0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)}
//                        };

float a = 0.2;
std::vector<glm::vec3> eq_aresta{glm::vec3(-0.5, -sqrt(3)/6, 0),
                                 glm::vec3(0.5,  -sqrt(3)/6, 0),
                                 glm::vec3(0, sqrt(3)/3, 0)};



//vector<VertexAttr> temp2{{glm::vec4(eq_aresta[0], 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
//                         {glm::vec4(eq_aresta[1], 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
//                         {glm::vec4(eq_aresta[2], 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},/*
//                         {glm::vec4(glm::vec2(eq_aresta[0]), -0.0, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
//                         {glm::vec4(glm::vec2(eq_aresta[2]), -0.0, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
//                         {glm::vec4(glm::vec2(eq_aresta[1]), -0.0, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
//                         {glm::vec4( .75f,  .75f,    0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
//                         {glm::vec4( 0.75f, -0.75f,  0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)},
//                         {glm::vec4(-0.75f, -0.75f,  0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)}*/
//                        };
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

      sphere1.initialize();
      sphere2.initialize();
      sphere3.initialize();
      cube1.initialize();
      cilinder1.initialize();

    gpuProgram.loadProgram("./hello.vert","./hello.frag");
    loc_attribute_Position =glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Position");
    loc_attribute_Color = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Color");
    gpuProgram.useProgram();
//    gpuProgram.programVarInfo();
    vbo[2].updateBufferData(temp2.data(),temp2.size()*sizeof(VertexAttr));

    vao->push<float>(loc_attribute_Position, 4);
    vao->push<float>(loc_attribute_Color, 4);
    vao->addBuffer(vbo[2]);
    _check_gl_error(__FILE__,__LINE__);

    cam.setMvpMatrixLoc(gpuProgram.getProgramID(), "mvpMatrix");
    cam.updateMvpMatrix();
    _check_gl_error(__FILE__, __LINE__);


//    printContextInformation();
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_CULL_FACE);
}



void Window::paintGL()
{
//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*EXAMPLE OF - DRAWING WITH glDrawArrays*/
    float bg[] = {0.0f,0.0f,0.0f,1.0};
    glClear( GL_DEPTH_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, bg);

    cilinder1.setProjectionMatrix(cam.projection());
    cilinder1.setMVMatrix(cam.view());
    cilinder1.render();




    cube1.setProjectionMatrix(cam.projection());
    cube1.setMVMatrix(cam.view());
    cube1.render();

//    glEnable(GL_CULL_FACE);

//    sphere2.setProjectionMatrix(cam.projection());
//    sphere2.setMVMatrix(cam.view());
//    sphere2.render();


//    sphere1.setProjectionMatrix(cam.projection());
//    sphere1.setMVMatrix(cam.view());
//    sphere1.render();

//    sphere3.setProjectionMatrix(cam.projection());
//    sphere3.setMVMatrix(cam.view());
//    sphere3.render();
//    glDisable(GL_CULL_FACE);




//    gpuProgram.useProgram();

//    cam.updateMvpMatrix();
//    vao->bind();
//    _check_gl_error(__FILE__, __LINE__);
////    unsigned int pidx[] = {0,1,2,3,4,5};
////    IndexBuffer idx((pidx), 6);
////    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    vao->unbind();
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

void Window::keyPressEvent(QKeyEvent *event)
{
    float angle = 1.f;
    switch(event->key())
    {
        case Qt::Key_Left: std::cout << "Tecla esquerda" << std::endl;
        cam.walkAround(glm::vec3(1.0,0.0,0.0));
        break;

        case Qt::Key_Right: std::cout << "Tecla Direita" << std::endl;
        cam.walkAround(glm::vec3(-1.0,0.0,0.0));
        break;

        case Qt::Key_Up: std::cout << "Tecla Cima" << std::endl;
        cam.walkAround(glm::vec3(0.0,1.0,0.0));
        break;

        case Qt::Key_Down: std::cout << "Tecla baixo" << std::endl;
        cam.walkAround(glm::vec3(0.0,-1.0,0.0));
        break;

        case Qt::Key_Plus: std::cout << "Tecla +" << std::endl;
        cam.zoomCommand(1.0);
        break;

        case Qt::Key_Minus: std::cout << "Tecla -" << std::endl;
        cam.zoomCommand(-1.0);
         break;

        case Qt::Key_D: std::cout << "Tecla D" << std::endl;
        cam.rotateObserver(glm::vec3(0.0f,1.0f,0.0f), angle);
         break;

        case Qt::Key_A: std::cout << "Tecla A" << std::endl;
        cam.rotateObserver(glm::vec3(0.0f,-1.0f,0.0f), angle);
         break;

    case Qt::Key_W: std::cout << "Tecla W" << std::endl;
    cam.rotateObserver(glm::vec3(-1.0f,0.0f,0.0f), angle);
     break;

    case Qt::Key_S: std::cout << "Tecla S" << std::endl;
    cam.rotateObserver(glm::vec3(1.0f,0.0f,0.0f), angle);
     break;

    case Qt::Key_J: std::cout << "Tecla J" << std::endl;
    cam.rotateObserver(glm::vec3(0.0f,0.0f,-1.0f), angle);
     break;

    case Qt::Key_K: std::cout << "Tecla K" << std::endl;
    cam.rotateObserver(glm::vec3(0.0f,0.0f,1.0f), angle);
     break;

    case Qt::Key_R: std::cout << "Tecla R" << std::endl;
    cam.resetCamera();
     break;



    }
}


void Window::resizeGL(int width, int height)
{

  // Currently we are not handling width/height changes
  //(void)width;
  //(void)height;
    cout << "(width, height): (" << width << " , " << height << ")" << endl;
if(width < height)
    glViewport(0, 0, width, width);
else
    glViewport(0, 0,  height, height);

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
