#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
//#include <QOpenGLVertexArrayObject>
#include <QDebug>
#include <QKeyEvent>
#include "./Utilities/Camera.h"
#include "LightSource.h"


#include "Models/QBall.h"
#include "Models/QBallRenderer.h"
#include <chrono>
#include <thread>


class QOpenGLShaderProgram;

class Window : public QOpenGLWindow
             , protected QOpenGLFunctions
{
  Q_OBJECT

// OpenGL Events
public:
  ~Window();


  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;
  inline void draw();
  void teardownGL();
  void show_variables();
  void addGlyph();

private:
  Camera m_camera;
  LightSource m_lightSource;
  int m_currentWidth, m_currentHeight;
  void viewPort();




  // OpenGL State Information
//    QOpenGLBuffer m_vertex[4];
//    QOpenGLVertexArrayObject m_object;
//    QOpenGLShaderProgram *m_program;

  // Private Helpers
  void printContextInformation();
protected:
//  void mousePressEvent(QMouseEvent *ev);
//  void moveEvent(QMoveEvent *ev);
//     void voidmouseMoveEvent(QMouseEvent *ev);
  void keyPressEvent(QKeyEvent *event) override;
//  void screenChanged(QScreen* event) override;
//  void keyReleaseEvent(QKeyEvent *event);
//  void mousePressEvent(QMouseEvent *event);
//  void mouseReleaseEvent(QMouseEvent *event);
};

#endif // WINDOW_H
