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
#include "Primitives/BSPLine.h"
#include "Primitives/BicubicBezierSurface.h"
#include "Primitives/RevolutionHermite.h"
#include "Primitives/IcoSphere.h"



//#include "QBallGlyphsCoefficientsSample.h"
#include <Utilities/Timer.h>


//QBall Q1(128*128, 4, 15, 15);
//QBallRenderer QR1;

QBallHemisphere Q1(4, 3, 15, 15);
QBallEvenRenderer QR1;


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
//      m_lightSource.setAmbient(glm::vec4(1.0f, 0.0f,0.0f,1.0f));
//      m_lightSource.setPosition(glm::vec4(1.0f, 1.0f,0.0f,1.0f));
//      m_lightSource.setShadingModel(LightSource::FLAT);

    Q1.computeODFs();
    QR1.initialize(&Q1);
    viewPort();

}



void Window::paintGL()
{

//    Timer T(__FUNCTION__);

//    float bg[] = {0.0f,0.0f,0.0f,1.0f};
//    glClear( GL_DEPTH_BUFFER_BIT);
//    glClearBufferfv(GL_COLOR, 0, bg);

////    renderCrossLine();

    draw();
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
        case Qt::Key_Left: //std::cout << "Tecla esquerda" << std::endl;
        m_camera.walkAround(glm::vec3(1.0,0.0,0.0));
        break;

        case Qt::Key_Right: //std::cout << "Tecla Direita" << std::endl;
        m_camera.walkAround(glm::vec3(-1.0,0.0,0.0));
        break;

        case Qt::Key_Up: //std::cout << "Tecla Cima" << std::endl;
        m_camera.walkAround(glm::vec3(0.0,1.0,0.0));
        break;

        case Qt::Key_Down: //std::cout << "Tecla baixo" << std::endl;
        m_camera.walkAround(glm::vec3(0.0,-1.0,0.0));
        break;

        case Qt::Key_Plus: //std::cout << "Tecla +" << std::endl;
        m_camera.zoomCommand(1.0);
        break;

        case Qt::Key_Minus: //std::cout << "Tecla -" << std::endl;
        m_camera.zoomCommand(-1.0);
         break;

        case Qt::Key_D: //std::cout << "Tecla D" << std::endl;
        m_camera.rotateObserver(glm::vec3(0.0f,1.0f,0.0f), angle);
         break;

        case Qt::Key_A: //std::cout << "Tecla A" << std::endl;
        m_camera.rotateObserver(glm::vec3(0.0f,-1.0f,0.0f), angle);
         break;

    case Qt::Key_W: //std::cout << "Tecla W" << std::endl;
    m_camera.rotateObserver(glm::vec3(-1.0f,0.0f,0.0f), angle);
     break;

    case Qt::Key_S: //std::cout << "Tecla S" << std::endl;
    m_camera.rotateObserver(glm::vec3(1.0f,0.0f,0.0f), angle);
     break;

    case Qt::Key_J: //std::cout << "Tecla J" << std::endl;
    m_camera.rotateObserver(glm::vec3(0.0f,0.0f,-1.0f), angle);
     break;

    case Qt::Key_K: //std::cout << "Tecla K" << std::endl;
    m_camera.rotateObserver(glm::vec3(0.0f,0.0f,1.0f), angle);
     break;

    case Qt::Key_R: //std::cout << "Tecla R" << std::endl;
    m_camera.resetCamera();
     break;

    case Qt::Key_G:
//    addGlyph();
     break;
    }

    draw();

//    renderCrossLine();



}


void Window::resizeGL(int width, int height)
{
    m_currentWidth = QPaintDevice::devicePixelRatio()*width;
    m_currentHeight = QPaintDevice::devicePixelRatio()*height;
//    cout << "(width, height): (" << width << " , " << height << ")" << endl;
    viewPort();
    draw();


}

void Window::draw()
{

    viewPort();
    float bg[] = {1.0f,1.0f,1.0f,1.0};
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, bg);

    QR1.setProjectionMatrix(m_camera.projection());
    QR1.setMVMatrix(m_camera.view());
    QR1.render();
    update();


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
