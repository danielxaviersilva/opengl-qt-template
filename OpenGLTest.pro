QT       += core gui opengl

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Primitives/cilinder.cpp \
        Primitives/cube.cpp \
        Primitives/sphere.cpp \
        Utilities/camera.cpp \
        Utilities/cgutilities.cpp \
        Utilities/indexbuffer.cpp \
        Utilities/shader.cpp \
        Utilities/vertexarray.cpp \
        Utilities/vertexbuffer.cpp \
        lightsource.cpp \
        main.cpp \
        window.cpp


INCLUDEPATH += "/usr/local/Cellar/glm/0.9.9.5/include" \
                "/usr/local/Cellar/glew/2.1.0_1/include/"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Primitives/cilinder.h \
    Primitives/cube.h \
    Primitives/sphere.h \
    Utilities/camera.h \
    Utilities/cgutilities.h \
    Utilities/indexbuffer.h \
    Utilities/shader.h \
    Utilities/vertexarray.h \
    Utilities/vertexbuffer.h \
    lightsource.h \
    openGLAdditionals.h \
    window.h

DISTFILES += \
    hello.frag \
    hello.vert \
    renderLightning.frag \
    renderLightning.vert \
    shader.frag \
    shader.vert \
