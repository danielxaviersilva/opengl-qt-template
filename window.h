#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
//#include <QOpenGLVertexArrayObject>
#include <QDebug>
#include <QKeyEvent>
//#include "openGLAdditionals.h"

class QOpenGLShaderProgram;

class Window : public QOpenGLWindow
             , protected QOpenGLFunctions
{
  Q_OBJECT

// OpenGL Events
public:
  ~Window();

  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void teardownGL();
  void show_variables();

private:
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
  void keyPressEvent(QKeyEvent *event);
//  void keyReleaseEvent(QKeyEvent *event);
//  void mousePressEvent(QMouseEvent *event);
//  void mouseReleaseEvent(QMouseEvent *event);
};

#endif // WINDOW_H
