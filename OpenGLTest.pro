QT       += core gui opengl

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) {
  message("Debug")
  DEFINES += TESTE1

}else{
  message("Release")
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Additionals/SphericalODF.cpp \
        LightSource.cpp \
        Models/QBall.cpp \
        Models/QBallEvenRenderer.cpp \
        Models/QBallHemisphere.cpp \
        Models/QBallRenderer.cpp \
        Primitives/BSPLine.cpp \
        Primitives/BicubicBezierSurface.cpp \
        Primitives/HermiteSweep.cpp \
        Primitives/IcoSphere.cpp \
        Primitives/RevolutionHermite.cpp \
        Primitives/SphereSet.cpp \
        Primitives/cilinder.cpp \
        Primitives/cube.cpp \
        Primitives/sphere.cpp \
        SphericalHarmonics.cpp \
        Utilities/CGUtilities.cpp \
        Utilities/Camera.cpp \
        Utilities/IndexBuffer.cpp \
        Utilities/Shader.cpp \
        Utilities/Texture.cpp \
        Utilities/Timer.cpp \
        Utilities/VertexArray.cpp \
        Utilities/VertexBuffer.cpp \
        main.cpp \
        window.cpp


QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -I/usr/local/include
QMAKE_LFLAGS += -lomp

INCLUDEPATH += "/usr/local/Cellar/glm/0.9.9.5/include" \
                "/usr/local/Cellar/glew/2.1.0_1/include/" \
#                "/Users/daniel_mac/Downloads/boost_1_75_0"

LIBS += -L /usr/local/lib /usr/local/lib/libomp.dylib
      #  "/Users/daniel_mac/Downloads/boost_1_75_0/stage/lib"
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Additionals/SphericalODF.h \
    LightSource.h \
    Models/QBall.h \
    Models/QBallEvenRenderer.h \
    Models/QBallHemisphere.h \
    Models/QBallRenderer.h \
    Primitives/BSPLine.h \
    Primitives/BicubicBezierSurface.h \
    Primitives/HermiteSweep.h \
    Primitives/IcoSphere.h \
    Primitives/RevolutionHermite.h \
    Primitives/SphereSet.h \
    Primitives/cilinder.h \
    Primitives/cube.h \
    Primitives/sphere.h \
    QBallGlyphsCoefficientsSample.h \
    SphericalHarmonics.h \
    Utilities/CGUtilities.h \
    Utilities/Camera.h \
    Utilities/IndexBuffer.h \
    Utilities/Shader.h \
    Utilities/Texture.h \
    Utilities/Timer.h \
    Utilities/VertexArray.h \
    Utilities/VertexBuffer.h \
    Utilities/renderCrossLine.h \
    openGLAdditionals.h \
    window.h

DISTFILES += \
    Isa.png \
    Mariana.png \
    Models/renderQBallEvenGlyphs.frag \
    Models/renderQBallEvenGlyphs.vert \
    Models/renderQBallGlyphs.frag \
    Models/renderQBallGlyphs.vert \
    SolidObjectDrawer.frag \
    SolidObjectDrawer.vert \
    hello.frag \
    hello.vert \
    mickey.jpg \
    renderInstances.frag \
    renderInstances.vert \
    renderLightning.frag \
    renderLightning.vert \
    renderQBallEvenGlyphs.frag \
    renderQBallEvenGlyphs.vert \
    renderQBallGlyphs.frag \
    renderQBallGlyphs.vert \
    shader.frag \
    shader.vert \


QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv4
