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




Window::~Window()
{
  makeCurrent();
  teardownGL();
//  delete Q1;
//  delete QR1;
}

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/
void Window::initializeGL()
{
    //CONSTANTS
    m_IcosahedronIterations = 4;
    m_isRenderingOptimized = true;
    m_benchmarkMaxSamplesAmount = 10000;
    m_executionsAmount = 100;
    m_step = 5;

//    m_odfSamplesAmount = 4096u;
    //INITIAL Values PARAMETERS
    m_rows = 1;
    m_odfSamplesAmount = m_rows*m_rows;
    m_counter = 0;
    m_indexAmount = m_odfSamplesAmount;
    m_glyphsIndex.resize(m_indexAmount);
    std::iota (std::begin(m_glyphsIndex), std::end(m_glyphsIndex), 0);
    //////

    initializeOpenGLFunctions();

//      m_lightSource.setAmbient(glm::vec4(1.0f, 0.0f,0.0f,1.0f));ss
//      m_lightSource.setPosition(glm::vec4(1.0f, 1.0f,0.0f,1.0f));
//      m_lightSource.setShadingModel(LightSource::FLAT);


    //Contrutores:
    //Primeiro argumento: Tamanho dos samples
    //Segundo argumento: Tesselagens do icosaedro
        // 0 (ordem 0): 12 vertices;
        // 1 (ordem 2): 42 vertices;
        // 2 (ordem 4): 162 vertices;
        // 3 (ordem 8): 642 vertices;
        // 4 (ordem 16): 2562 vertices;
    //Terceiro/quarto argumento: theta e phi para discretizar mesh esférico. Somente util quando a variavel isIco do construtor é false.





   //QBallHemisphere e EvenRenderer armazenam renderizam dados da forma escrita no artigo na subseção da renderização de ODFs simétricas e são mais rápidas
   if (m_isRenderingOptimized) {
        m_Qball1 = new QBallHemisphere(m_odfSamplesAmount, m_IcosahedronIterations, 15, 15);
        m_QballRenderer1 = new QBallEvenRenderer();
        m_file.setFileName(QString::fromStdString("/Users/daniel_mac/Half_V" + std::to_string(m_Qball1->getBaseDirections().size()) + ".txt"));
   } else {
        m_Qball1 = new QBall(m_odfSamplesAmount, m_IcosahedronIterations, 15, 15);
        m_QballRenderer1 = new QBallRenderer();
        m_file.setFileName(QString::fromStdString("/Users/daniel_mac/Full_V" + std::to_string(m_Qball1->getBaseDirections().size()) + ".txt"));
   }


    m_file.open(QIODevice::WriteOnly | QIODevice::Text);
    m_Qball1->computeODFs();
    m_QballRenderer1->initialize(m_Qball1);
    resizeGL(this->width(), this->height());
    m_startTimePoint = std::chrono::high_resolution_clock::now();

}



void Window::paintGL()
{


//    m_counter = m_counter%m_odfSamplesAmount;

//    Timer T(__FUNCTION__);

//    float bg[] = {0.0f,0.0f,0.0f,1.0f};
//    glClear( GL_DEPTH_BUFFER_BIT);
//    glClearBufferfv(GL_COLOR, 0, bg);

////    renderCrossLine();

    draw();
    update();


    if(m_counter == m_executionsAmount) {
        delete m_Qball1;
        delete m_QballRenderer1;
//         m_file.open(QIODevice::WriteOnly | QIODevice::Text);
        updateBenchmarkParameters();
        m_startTimePoint = std::chrono::high_resolution_clock::now();
        return;
    }
    m_counter++;


    m_endTimePoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTimePoint).time_since_epoch().count();
    auto duration = end - start;
//    double ms = duration*0.001;
    QTextStream out(&m_file);
    out << m_glyphsIndex.size() << ", " << duration << "\n";
//    std::cout << m_glyphsIndex.size() << ", " << duration << std::endl;
    m_startTimePoint = std::chrono::high_resolution_clock::now();

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

void Window::updateBenchmarkParameters()
{


    m_rows +=m_step;
    m_odfSamplesAmount = m_rows*m_rows;
    m_counter = 0;
    //m_executionsAmount = 30;
    m_indexAmount = m_odfSamplesAmount;
    m_glyphsIndex.resize(m_indexAmount);
    std::iota (std::begin(m_glyphsIndex), std::end(m_glyphsIndex), 0);

    if ( m_odfSamplesAmount > m_benchmarkMaxSamplesAmount ) {
        std::cout << "BENCHMARK FINISHED!" << std::endl;
        exit(0);
    }

    if ( m_isRenderingOptimized ) {
        m_Qball1 =  new QBallHemisphere(m_odfSamplesAmount, m_IcosahedronIterations, 15, 15);
        m_QballRenderer1 = new QBallEvenRenderer();
    } else {
        m_Qball1 = new QBall(m_odfSamplesAmount, m_IcosahedronIterations, 15, 15);
        m_QballRenderer1 = new QBallRenderer();
    }
    m_Qball1->computeODFs();
    m_QballRenderer1->initialize(m_Qball1);

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
    m_QballRenderer1->setProjectionMatrix(m_camera.projection());
    m_QballRenderer1->setMVMatrix(m_camera.view());
    draw();

//    renderCrossLine();



}


void Window::resizeGL(int width, int height)
{
    m_currentWidth = QPaintDevice::devicePixelRatio()*width;
    m_currentHeight = QPaintDevice::devicePixelRatio()*height;
    std::cout << "(m_currentWidth , m_currentHeight): (" << width << " , " << height << ")" << std::endl;
    _check_gl_error(__FILE__, __LINE__);
    viewPort();
    _check_gl_error(__FILE__, __LINE__);
    draw();


}

void Window::draw()
{
    //m_counter é incrementado em paintGL




    viewPort();
    float bg[] = {1.0f,1.0f,1.0f,1.0};
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, bg);
    //Timer - begin
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint = std::chrono::high_resolution_clock::now();
    //
    m_QballRenderer1->render(m_glyphsIndex);
    //
    //Timer - end







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
