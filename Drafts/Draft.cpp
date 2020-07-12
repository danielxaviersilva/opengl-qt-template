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


Shader gpuProgram;
Shader gpuLightSphere;
//uint vbo[2];

uint vbo_vc;
uint vbo_temp3;

VertexBuffer *vbo = new VertexBuffer[3];
VertexArray *vao = new VertexArray[1];




Cube cube1(0.25,glm::vec3(0.5,0.5,0.0), glm::vec3(0.0,1.0,0.0));
Cilinder cilinder1(50, 50,0.5f, 0.7f,glm::vec3(0.0),glm::vec3(0.0, 1.0, 0.0));

SphereSet SS1(100,100);




// Create a colored triangle
Sphere sphere2(25,25,0.125, glm::vec3(-0.0f,0.0f, 0.0f));



vector<float> vertices =  { .75f,  .75f,   0.0f, 1.0f,
                            0.75f, -0.75f, 0.0f, 1.0f,
                           -0.75f, -0.75f, 0.0f, 1.0f};

vector<float>  color ={ 1.0f, 1.0f, 1.0f, 0.5f,
                        1.0f, 1.0f, 1.0f, 0.5f,
                        1.0f, 1.0f, 1.0f, 0.5f};

typedef struct _scaleOffset{
    float scale;
    glm::vec3 offset;
} scaleOffset;


typedef struct _vertexAttr{
   glm::vec4 vertex;
   glm::vec4 color;
   glm::vec2 texCoord;
   _vertexAttr(glm::vec4 inVertex, glm::vec4 inColor, glm::vec2 inTexCoord): vertex(inVertex), color(inColor), texCoord(inTexCoord){};

//   float vertex[4];
//   float color[4];
//   _vertexAttr(float *inVertex, float *inColor): vertex(inVertex), color(inColor){};

} VertexAttr;

float a = 0.2;
std::vector<glm::vec3> eq_aresta{glm::vec3(-0.5, -sqrt(3)/6, 0),
                                 glm::vec3(0.5,  -sqrt(3)/6, 0),
                                 glm::vec3(0, sqrt(3)/3, 0)};

vector<VertexAttr> temp2{{glm::vec4(-1.0f,-1.0f, 0.0f,  1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f,1.0f)},
                         {glm::vec4(1.0f,-1.0f, 0.0f,  1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f,1.0f)},
                         {glm::vec4(1.0f,1.0f, 0.0f,  1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f,0.0f)},
                         {glm::vec4(-1.0f,1.0f, 0.0f,  1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f)}/*
*/
                        };
uint VAO[3];

int loc_attribute_Position;
int loc_attribute_Color;
int loc_attribute_texCoord;
int loc_displacement;
int loc_scale;


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
      test.uploadTexture("./mickey.jpg");

      SS1.initialize();
      SS1.addSphere(0.25, glm::vec3(-0.0f,0.0f,0.0f));
      SS1.addSphere(0.125, glm::vec3(0.5f,-0.5f,0.0f), glm::vec3(1.0,1.0,0.0));
//      SS1.setMaterialLighting(glm::vec4(1.0f,0.71f ,.75f, 1.0f),glm::vec4(1.0f,0.71f ,.75f, 1.0f), glm::vec4(0.6f,.6f ,.6f, 1.0f), 25.0f);
//      SS1.setLightSource();
//      SS1.addSphere(0.25, glm::vec3(-0.5f,-0.5f,0.0f));
//      SS1.addSphere(0.25, glm::vec3(0.5f,0.5f,0.0f));
//      SS1.addSphere(0.25, glm::vec3(0.5f,-0.5f,0.0f));

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

      cube1.initialize();
      cilinder1.initialize();

    gpuProgram.loadProgram("./hello.vert","./hello.frag");
    loc_attribute_Position =glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Position");
    loc_attribute_Color = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_Color");
    loc_attribute_texCoord = glGetAttribLocation(gpuProgram.getProgramID(),"attribute_texCoord")/*FAZER VARIAVEL*/;
    loc_displacement = glGetAttribLocation(gpuProgram.getProgramID(),"displacement");
    loc_scale = glGetAttribLocation(gpuProgram.getProgramID(),"scale");

    gpuProgram.useProgram();

    int temp = glGetUniformLocation(gpuProgram.getProgramID(), "u_Texture");
    glUniform1i(temp, slot);

//    gpuProgram.programVarInfo();

    std::vector<scaleOffset> m_scaleOffset{
        {1.0f,glm::vec3(0.0f,0.0f,0.0f)},
        {0.5f,glm::vec3(-0.5f, 0.0f,-0.5f)},
        {0.5f,glm::vec3( 0.5f, 0.0f,-0.5f)},
        {0.3f,glm::vec3( 0.0f,-0.5f,-0.5f)},
        {0.3f,glm::vec3( 0.0f, 0.5f,-0.5f)}
    };


    vbo[0].updateBufferData(temp2.data(),temp2.size()*sizeof(VertexAttr));
    vbo[1].updateBufferData(m_scaleOffset.data(), m_scaleOffset.size()*sizeof(scaleOffset));

    vao->push<float>(loc_attribute_Position, 4);
    vao->push<float>(loc_attribute_Color, 4);
    vao->push<float>(loc_attribute_texCoord, 2);
    vao->addBuffer(vbo[0]);
    vao->clearLayout();
    vao->push<float>(loc_scale, 1, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    vao->push<float>(loc_displacement, 3, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    vao->addBuffer(vbo[1]);
    _check_gl_error(__FILE__,__LINE__);

    m_camera.setMvpMatrixLoc(gpuProgram.getProgramID(), "mvpMatrix");
    m_camera.updateMvpMatrix();
    _check_gl_error(__FILE__, __LINE__);


//    printContextInformation();
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
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

//    cilinder1.setProjectionMatrix(cam.projection());
//    cilinder1.setMVMatrix(cam.view());
//    cilinder1.render();

//    cube1.setProjectionMatrix(cam.projection());
//    cube1.setMVMatrix(cam.view());
//    cube1.render();

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

//    SS1.setProjectionMatrix(m_camera.projection());
//    SS1.setMVMatrix(m_camera.view());
//    SS1.render();

   ODF1.setProjectionMatrix(m_camera.projection());
   ODF1.setMVMatrix(m_camera.view());
   ODF1.render();




//    gpuProgram.useProgram();

//    m_camera.updateMvpMatrix();
//    vao->bind();
//    test.Bind(slot);
//    _check_gl_error(__FILE__, __LINE__);
//    unsigned int pidx[] = {0,1,2, 0, 2, 3};
//    IndexBuffer idx((pidx), 6);
//    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 5);
////    glDrawArrays(GL_TRIANGLES, 0, 6);
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
