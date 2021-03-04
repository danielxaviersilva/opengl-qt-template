#ifndef QBALLEVENRENDERER_H
#define QBALLEVENRENDERER_H
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <OpenGL.h>
#include <QGLFunctions>
#include "../Utilities/VertexArray.h"
#include "../Utilities/VertexBuffer.h"
#include "../Utilities/IndexBuffer.h"
#include "../Utilities/Shader.h"
#include "../Utilities/CGUtilities.h"
#include "../LightSource.h"
#include "../Utilities/Texture.h"

#include "QBallRenderer.h"


class QBallEvenRenderer: public QBallRenderer
{
public:
    QBallEvenRenderer();


    // QBallRenderer interface
public:
    void initialize(QBall *qBall);
    void render();

protected:
    void setInstancedVertexAttribBuffer();
};

#endif // QBALLEVENRENDERER_H
