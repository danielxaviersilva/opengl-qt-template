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




using namespace std;



int t = 15, f = 15;
SphereODF ODF1(t, f);
Texture test;
int slot = 0;

//SphereSet s1(4,3);
IcoSphere t1(313);
SphereSet s1(15,15);

BSPLine bsp1;
BSPLine bsp2;
BSPLine bsp3;

BicubicBezierSurface bz1;
RevolutionHermite rh1;

QBall Q1(197, 1, 15, 15);
QBallRenderer QR1(15, 15);

Cilinder c1;


Window::~Window()
{
  makeCurrent();
  teardownGL();
}

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/
void normalize(std::vector<float> &input, bool verbose = false)
{
    std::vector<int> nonValidIdx;
    int idx = 0;
    for(auto& element : input){
        if(element < 0){
            element = 0.03f;
            nonValidIdx.push_back(idx);
        }
        idx++;
    }

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    idx = 0;
    for(auto& element : input){
        if(!std::count(nonValidIdx.begin(), nonValidIdx.end(), idx))
        {
            if(element > max)
                max = element;
            if(element < min)
                min = element;
        }
        idx++;
    }
    idx = 0;
    for(auto& element : input){
        if(!std::count(nonValidIdx.begin(), nonValidIdx.end(), idx))
            element = (element - min)/(max - min);
        idx++;
    }

    if(verbose)
    {
        int idx = 0;
        for(auto& element : input)
            std::cout << idx++ << ": " << element << std::endl;
    }


}

void absoluteNormalize(std::vector<float> &input, bool verbose = false)
{
    std::vector<int> nonValidIdx;
    int idx = 0;
    for(auto& element : input){
        element = fabs(element);
    }

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    idx = 0;
    for(auto& element : input){
        if(!std::count(nonValidIdx.begin(), nonValidIdx.end(), idx))
        {
            if(element > max)
                max = element;
            if(element < min)
                min = element;
        }
        idx++;
    }
    idx = 0;
    for(auto& element : input){
        if(!std::count(nonValidIdx.begin(), nonValidIdx.end(), idx))
            element = (element - min)/(max - min);
        idx++;
    }

    if(verbose)
    {
        int idx = 0;
        for(auto& element : input)
            std::cout << idx++ << ": " << element << std::endl;
    }


}

void minMaxnormalize(std::vector<float> &input, bool verbose = false)
{

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    for(auto& element : input){

            if(element > max)
                max = element;
            if(element < min)
                min = element;
    }

    for(auto& element : input){
            element = (element - min)/(max - min);
    }
    if(verbose)
    {
        int idx = 0;
        for(auto& element : input)
            std::cout << idx++ << ": " << element << std::endl;
    }


}


void Window::initializeGL()
{

      initializeOpenGLFunctions();
      c1.initialize();



      m_lightSource.setAmbient(glm::vec4(1.0f, 0.0f,0.0f,1.0f));
      m_lightSource.setPosition(glm::vec4(1.0f, 1.0f,0.0f,1.0f));
      m_lightSource.setShadingModel(LightSource::FLAT);

//      c1.setLighting(&m_lightSource);


//    rh1.initialize();
//    rh1.setColor(glm::vec4(1.0f, 0.0f, 0.0f,1.0f));

//    std::vector<float> test;
////    test = Psi679871;
//    test = Psi679962;


//        Texture t;

//    for(auto & elTest : test)
//    {
//        elTest = 1.0f;
//    }



//     ODF1.initialize();
//      absoluteNormalize(test, false);
//     minMaxnormalize(test, true);

//      normalize(test,false);

//           test[0] = test[181] = 1.0f;

//      for (auto& el : test)
//          el = .5f;
//      for (int i = 0; i< 50; i++)
//          test.push_back(0.5f);

//      ODF1.addGlyph(test, glm::vec3(0.f));


//      ODF1.addThreeAngleGlyph(test, 0.25f);
      QR1.initialize(&Q1);

//    _check_gl_error(__FILE__, __LINE__);

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    draw();
    viewPort();

}



void Window::paintGL()
{
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));

//    Timer T(__FUNCTION__);

    /*EXAMPLE OF - DRAWING WITH glDrawArrays*/
//    viewPort();
    float bg[] = {0.0f,0.0f,0.0f,1.0f};
    glClear( GL_DEPTH_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, bg);

////    renderCrossLine();


//   ODF1.setProjectionMatrix(m_camera.projection());
//   ODF1.setMVMatrix(m_camera.view());
//   ODF1.render();


//    update();
    draw();
//    c1.setProjectionMatrix(m_camera.projection());
//    c1.setMVMatrix(m_camera.view());
//    c1.render();
    update();
//    viewPort();
//    float bg[] = {1.0f,1.0f,1.0f,1.0};
//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glClearBufferfv(GL_COLOR, 0, bg);
//    QR1.setProjectionMatrix(m_camera.projection());
//    QR1.setMVMatrix(m_camera.view());
//    QR1.render();
//    update();


}

void Window::teardownGL()
{
    glUseProgram(0);
    exit(0);
}

void Window::addGlyph()
{
    std::vector<float> profile;
    profile.resize(197);
    for (auto &p:profile )
        p = ((float)rand()/(float)(RAND_MAX));


    ODF1.addGlyph(profile, glm::vec3(2*((float)rand()/(float)(RAND_MAX))-1,
                                     2*((float)rand()/(float)(RAND_MAX))-1,
                                     2*((float)rand()/(float)(RAND_MAX))-1), glm::vec3(0.0f,1.0f,0.0f), 0.1f);

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
    cout << "(width, height): (" << width << " , " << height << ")" << endl;
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



//    ODF1.setProjectionMatrix(m_camera.projection());
//    ODF1.setMVMatrix(m_camera.view());
//    ODF1.render();
//     update();

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
