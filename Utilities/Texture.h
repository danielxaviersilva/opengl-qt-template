#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <string>
//#include <GL/glew.h>
#include <QGLFunctions>
#include "CGUtilities.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/types_c.h>

using namespace  cv;


class Texture : protected CGUtilities
{
    unsigned int m_rendererID;
    std::string m_path;
    unsigned char *m_localBuffer;
    int m_width, m_height, m_BPP;
    bool m_initialized;

public:
    Texture();
    Texture(const std::string& path);
    ~Texture();

    void uploadTexture(const std::string &path);

    void Bind (unsigned int slot = 0) const;
    void Unbind() const;




    int getWidth() const;
    int getHeight() const;
    Texture(std::vector<float> data, int width, int height);
    void uploadTexture(std::vector<float> data, int width, int height);
    void deleteTexture();
};

#endif // TEXTURE_H
