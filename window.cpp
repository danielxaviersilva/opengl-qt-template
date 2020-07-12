#include "window.h"
#include <iostream>

#include "openGLAdditionals.h"


#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>
#include "./Utilities/Shader.h"
#include "./Utilities/IndexBuffer.h"
#include "./Utilities/VertexBuffer.h"
#include "./Utilities/VertexArray.h"

#include "./Utilities/Texture.h"

#include "./Primitives/sphere.h"
#include "./Primitives/cube.h"
#include "./Primitives/cilinder.h"

#include "./Primitives/SphereSet.h"

#include"Additionals/SphericalODF.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Utilities/renderCrossLine.h"



#include "QBallGlyphsCoefficientsSample.h"

using namespace std;



int t = 11, f = 12;
SphereODF ODF1(t, f);
Texture test;
int slot = 0;



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


      m_lightSource.setAmbient(glm::vec4(1.5f, 0.5f,0.5f,1.0f));
      m_lightSource.setPosition(glm::vec4(0.0f, 0.0f,0.0f,1.0f));
      m_lightSource.setShadingModel(LightSource::GOURAUD);
      ODF1.initialize(&m_lightSource);


      float min = *min_element(Psi874797.begin(), Psi874797.end());
      float max = *max_element(Psi874797.begin(), Psi874797.end());

      for(auto& element : Psi874797)
          element = (element - min)/(max - min);

//      for (auto &psi : Psi874797)
//          psi = pow(psi, 3);

      ODF1.addThreeAngleGlyph(Psi874797, 0.5f);



//      ODF1.addGlyph(testODF1, glm::vec3( 0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);
//      ODF1.addGlyph(testODF2, glm::vec3(-0.5f, -0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);
//      ODF1.addGlyph(ODFpar3, glm::vec3(-0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);

//    ODF1.addGlyph(psi29016, glm::vec3( 0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);
//    ODF1.addGlyph(psi15668, glm::vec3(-0.5f, -0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);



//    ODF1.addGlyph(Psi293, glm::vec3( 0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);
//    ODF1.addGlyph(Psi5040, glm::vec3( -0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);




//          std::reverse(ODF.begin(),ODF.end());


//      ODF1.addSphere(ODFpar3,glm::vec3(.5f));

//    gpuProgram.programVarInfo();


    _check_gl_error(__FILE__,__LINE__);


    _check_gl_error(__FILE__, __LINE__);


//    printContextInformation();
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void Window::paintGL()
{
//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*EXAMPLE OF - DRAWING WITH glDrawArrays*/
    viewPort();
    float bg[] = {0.0f,0.0f,0.0f,1.0};
    glClear( GL_DEPTH_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, bg);

    renderCrossLine();


   ODF1.setProjectionMatrix(m_camera.projection());
   ODF1.setMVMatrix(m_camera.view());
   ODF1.render();

    update();


}

void Window::teardownGL()
{
    glUseProgram(0);
    exit(0);
}

void Window::viewPort()
{
    if(m_currentWidth < m_currentHeight)
        glViewport(0, (m_currentHeight-m_currentWidth)/2, m_currentWidth, m_currentWidth);
    else
        glViewport((m_currentWidth-m_currentHeight)/2, 0, m_currentHeight, m_currentHeight);

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
        m_camera.walkAround(glm::vec3(1.0,0.0,0.0));
        break;

        case Qt::Key_Right: std::cout << "Tecla Direita" << std::endl;
        m_camera.walkAround(glm::vec3(-1.0,0.0,0.0));
        break;

        case Qt::Key_Up: std::cout << "Tecla Cima" << std::endl;
        m_camera.walkAround(glm::vec3(0.0,1.0,0.0));
        break;

        case Qt::Key_Down: std::cout << "Tecla baixo" << std::endl;
        m_camera.walkAround(glm::vec3(0.0,-1.0,0.0));
        break;

        case Qt::Key_Plus: std::cout << "Tecla +" << std::endl;
        m_camera.zoomCommand(1.0);
        break;

        case Qt::Key_Minus: std::cout << "Tecla -" << std::endl;
        m_camera.zoomCommand(-1.0);
         break;

        case Qt::Key_D: std::cout << "Tecla D" << std::endl;
        m_camera.rotateObserver(glm::vec3(0.0f,1.0f,0.0f), angle);
         break;

        case Qt::Key_A: std::cout << "Tecla A" << std::endl;
        m_camera.rotateObserver(glm::vec3(0.0f,-1.0f,0.0f), angle);
         break;

    case Qt::Key_W: std::cout << "Tecla W" << std::endl;
    m_camera.rotateObserver(glm::vec3(-1.0f,0.0f,0.0f), angle);
     break;

    case Qt::Key_S: std::cout << "Tecla S" << std::endl;
    m_camera.rotateObserver(glm::vec3(1.0f,0.0f,0.0f), angle);
     break;

    case Qt::Key_J: std::cout << "Tecla J" << std::endl;
    m_camera.rotateObserver(glm::vec3(0.0f,0.0f,-1.0f), angle);
     break;

    case Qt::Key_K: std::cout << "Tecla K" << std::endl;
    m_camera.rotateObserver(glm::vec3(0.0f,0.0f,1.0f), angle);
     break;

    case Qt::Key_R: std::cout << "Tecla R" << std::endl;
    m_camera.resetCamera();
     break;
    }
}


void Window::resizeGL(int width, int height)
{
    m_currentWidth = QPaintDevice::devicePixelRatio()*width;
    m_currentHeight = QPaintDevice::devicePixelRatio()*height;
    cout << "(width, height): (" << width << " , " << height << ")" << endl;


}

//int i = 0;
//void Window::voidmouseMoveEvent(QM    ouseEvent *ev)
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
