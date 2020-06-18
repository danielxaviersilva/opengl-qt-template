#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <QOpenGLFunctions>
#include <vector>
#include "vertexbuffer.h"


class VertexArray
{
    typedef struct _layout{
        unsigned int varLocation;
        unsigned int elementCount;
        unsigned int type;
        bool normalized;
    } layout;

    std::vector<layout> m_layout;
    unsigned int m_vao;
    unsigned int m_stride;
    bool m_initialized;
public:
    VertexArray();
    ~VertexArray();
    void bind();
    void unbind();

    void addBuffer(VertexBuffer &vb);


    /*Buffer layout methods*/
    template<typename T>
    void push(unsigned int varLocation, unsigned int elementCount, bool normalized);
    template<typename T>
    void push(unsigned int varLocation, unsigned int elementCount);

    template<>
    void push<float>(unsigned int varLocation, unsigned int elementCount, bool normalized);
    template<>
    void push<float>(unsigned int varLocation, unsigned int elementCount);

    template<>
    void push<unsigned int> (unsigned int varLocation, unsigned int elementCount);
    template<>
    void push<unsigned int>(unsigned int varLocation, unsigned int elementCount, bool normalized);

    template<>
    void push<unsigned char> (unsigned int varLocation, unsigned int elementCount);
    template<>
    void push<unsigned char>(unsigned int varLocation, unsigned int elementCount, bool normalized);



};

#endif // VERTEXARRAY_H
