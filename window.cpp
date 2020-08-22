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



#include "QBallGlyphsCoefficientsSample.h"

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

void Window::initializeGL()
{
      initializeOpenGLFunctions();


//      m_lightSource.setAmbient(glm::vec4(1.5f, 0.5f,0.5f,1.0f));
//      m_lightSource.setPosition(glm::vec4(0.0f, 0.0f,0.0f,1.0f));
//      m_lightSource.setShadingModel(LightSource::FLAT);


    rh1.initialize();
    rh1.setColor(glm::vec4(1.0f, 0.0f, 0.0f,1.0f));





     ODF1.initialize();
      std::vector<float> test = Psi2457726;
      normalize(test, false);

//      for (auto& el : test)
//          el = .5f;

//      ODF1.addGlyph(test, glm::vec3(0.f));


      ODF1.addThreeAngleGlyph(test, 0.5f);

//      s1.initialize();
//      s1.addSphere();
//      s1.setMaterialLighting(3.0f*glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
//                                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
//                                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        t1.initialize();
        t1.addSphere();
        t1.setMaterialLighting(3.0f*glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
//      for (auto &psi : Psi874797)
//          psi = pow(psi, 3);





//      ODF1.addGlyph(test, glm::vec3( 0.5f,  0.5, 0.0f) , glm::vec3(0.0f,0.0f,1.0f), 0.5f);
//      ODF1.addGlyph(testODF2, glm::vec3(-0.5f, -0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);
//      ODF1.addGlyph(ODFpar3, glm::vec3(-0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);

//    ODF1.addGlyph(psi29016, glm::vec3( 0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);
//    ODF1.addGlyph(psi15668, glm::vec3(-0.5f, -0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);



//    ODF1.addGlyph(Psi293, glm::vec3( 0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);
//    ODF1.addGlyph(Psi5040, glm::vec3( -0.5f,  0.5, 0.0f) , glm::vec3(0.0f,1.0f,0.0f), 0.5f);




//          std::reverse(ODF.begin(),ODF.end());


//      ODF1.addSphere(ODFpar3,glm::vec3(.5f));

//    gpuProgram.programVarInfo();
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

//    renderCrossLine();


   ODF1.setProjectionMatrix(m_camera.projection());
   ODF1.setMVMatrix(m_camera.view());
   ODF1.render();

//   s1.setProjectionMatrix(m_camera.projection());
//   s1.setMVMatrix(m_camera.view());
//   s1.render();

    t1.setProjectionMatrix(m_camera.projection());
    t1.setMVMatrix(m_camera.view());
    t1.render();

//    bsp1.setProjectionMatrix(m_camera.projection());
//    bsp1.setMVMatrix(m_camera.view());
//    bsp1.render();

//    bsp2.setProjectionMatrix(m_camera.projection());
//    bsp2.setMVMatrix(m_camera.view());
//    bsp2.render();

//    bsp3.setProjectionMatrix(m_camera.projection());
//    bsp3.setMVMatrix(m_camera.view());
//    bsp3.render();


//    bz1.setProjectionMatrix(m_camera.projection());
//    bz1.setMVMatrix(m_camera.view());
//    bz1.render();

//      rh1.setProjectionMatrix(m_camera.projection());
//      rh1.setMVMatrix(m_camera.view());
//      rh1.render();





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
