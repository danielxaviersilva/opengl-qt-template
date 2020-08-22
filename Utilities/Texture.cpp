#include "Texture.h"

Texture::Texture():
m_rendererID(0), m_path(""), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0), m_initialized(false)
{

}

Texture::Texture(const std::string &path):
m_rendererID(0), m_path(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0), m_initialized(false)
{
    uploadTexture(path);

}

Texture::Texture(std::vector<float> data, int width, int height):
m_rendererID(0), m_path(""), m_localBuffer(nullptr), m_width(width), m_height(height), m_BPP(0), m_initialized(false)
{
    uploadTexture(data, width, height);

}

void Texture::uploadTexture(std::vector<float> data, int width, int height)
{
    if (!m_initialized){
    glGenTextures(1, &m_rendererID);
    m_initialized = true;
    }
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    _check_gl_error(__FILE__,__LINE__);




    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width, height,0, GL_RED, GL_FLOAT, data.data());
    _check_gl_error(__FILE__,__LINE__);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::deleteTexture()
{
 glDeleteTextures(1, &m_rendererID);
 m_initialized = false;
}


void Texture::uploadTexture(const std::string &path)
{

    cv::Mat image = cv::imread(path.c_str());
    if(image.empty()){
          std::cerr << "Texture::Texture(): Texture not load. Path is invalid." << std::endl;
          return;
    }

    if (!m_initialized){
    glGenTextures(1, &m_rendererID);
    m_initialized = true;
    }
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    _check_gl_error(__FILE__,__LINE__);




    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image.cols, image.rows,0, GL_BGR,GL_UNSIGNED_BYTE, image.ptr());
    _check_gl_error(__FILE__,__LINE__);
    glBindTexture(GL_TEXTURE_2D, 0);

    image.release();

}

Texture::~Texture()
{
    glDeleteTextures(1,&m_rendererID);
}



void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


int Texture::getHeight() const
{
    return m_height;
}



int Texture::getWidth() const
{
    return m_width;
}
