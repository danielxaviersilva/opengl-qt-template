#include "BSPLine.h"
std::vector<float>  BSPLine::setBSPLineDomain(const std::vector<float> &controlPointSet, const std::vector<float> &nodes, const int &degree, const int &resolution)
{
    std::vector<float> BSPline;

    if(nodes.size() <controlPointSet.size() + degree)
    {
        std::cerr << "BSPLine::setBSPLineDomain: Node vectors too small compared to controlPoints" << std::endl;
        return BSPline;
    }

    std::vector<float> t_domain;// = new float[resolution];

     float **N = new float *[(nodes.size()-1)];
     for (unsigned int i = 0; i < nodes.size() - 1; i++)
        N[i] = new float[resolution];

     std::vector<float> X_aux;// = new float [controlPointSet.size() + degree];

     for (unsigned int i = 0; i<controlPointSet.size() + degree; i++)
                 X_aux.push_back(nodes[i]);

     for (int i = 0; i < resolution; i++)
        t_domain.push_back(X_aux[0] + i*(X_aux[X_aux.size() - 1] - X_aux[0])/(resolution - 1));


     for (unsigned int i = 0; i < X_aux.size() - 1; i++)
     {
         for (int j = 0; j < resolution; j++)
         {
             if(t_domain[j] >= nodes[i] && t_domain[j] < nodes[i+1])
                 N[i][j] = 1;
             else
                 N[i][j] = 0;
         }
     }

     int gr = 2;
     for (gr = 2; gr <= degree; gr++)
     {
         //laço para gerar as funções para um auxiliar
         for (unsigned int i = 0; i < X_aux.size() - gr; i++)
         {
             //Laço que gera os valoresolution da função
             //System.out.println("i: " + i);
             //System.out.println(X_aux[i + gr]);
             for (int t = 0; t < resolution; t++)
             {
                 N[i][t] = (t_domain[t] - X_aux[i])/(X_aux[i + gr - 1] - nodes[i])*N[i][t] +
                 (X_aux[i + gr] - t_domain[t])/(X_aux[i + gr] - nodes[i+1])*N[i+1][t];

                // System.out.println("	t: " + t);
             }
         }
     }


     std::vector<float> BSPline_aux;// = new float [resolution];

     BSPline_aux.resize(resolution);
     for (auto & element : BSPline_aux)
         element = 0.0f;


     for (unsigned int i = 0; i < nodes.size() - gr && i < controlPointSet.size(); i++)
     {
//         System.out.println("i: " + i);
         for (int j = 0; j < resolution; j++)
         {
             //System.out.println("j: " + j);
             BSPline_aux[j] = BSPline_aux[j] + controlPointSet[i]*N[i][j];
         }
     }


     //Tirar os elementos da extremidade da BSP line que estão fora do range no aumento de graus
     int count = 0;

     //contar a dimensão do meu vetor
     for (unsigned int i = 0; i< t_domain.size(); i++)
     {
         if (t_domain[i] >= X_aux[degree - 1] && t_domain[i] <= X_aux[(X_aux.size() - 1) - (degree - 1)])
             count++;
     }


     int idx_BSP = 0;
     for (unsigned int i = 0; i< t_domain.size(); i++)
     {
         if (t_domain[i] >= X_aux[degree - 1] && t_domain[i] <= X_aux[(X_aux.size() - 1) - (degree - 1)])
         {
             BSPline.push_back(BSPline_aux[i]);
//             idx_BSP++;
         }
     }



     for (unsigned int i = 0; i < nodes.size() - 1; i++)
        delete [] N[i];
      delete[] N;

        return BSPline;

}

BSPLine::BSPLine() : m_initialized(false)
{

}

void BSPLine::initialize()
{

    if(!m_initialized)
    {

        m_initialized = true;
        m_program.loadProgram("./SolidObjectDrawer.vert","./SolidObjectDrawer.frag");
        m_program.useProgram();

        int locVertex =glGetAttribLocation(m_program.getProgramID(),"vertex");

        if(locVertex == -1)
        {
            std::cerr << "BSPLINE::initialize: At least one of the input names couldn't be found" << std::endl;
        }

        m_vao.bind();

        m_vao.push<float>(locVertex, 4);
        //Calculate the vertices and assign to buffer m_vbo
        m_vbo.genBuffer();

        m_vao.addBuffer(m_vbo);
        m_vao.unbind();
        m_program.release();
    }


}

void BSPLine::setBSPLine(std::vector<glm::vec2> controlPointSet, std::vector<float> nodes, int degree, float height, int resolution)
{

    std::vector<float> controlX;
    std::vector<float> controlY;

    for (auto point : controlPointSet)
    {
        controlX.push_back(point.x);
        controlY.push_back(point.y);
    }

    std::vector<float> X = setBSPLineDomain(controlX, nodes, degree, resolution);
    std::vector<float> Y = setBSPLineDomain(controlY, nodes, degree, resolution);

   std::vector<glm::vec4> surface;

   for (unsigned int i = 0; i < X.size() - 1; i++)
   {
       surface.push_back(glm::vec4(X[i], Y[i], 0.0f, 1.0f));
       surface.push_back(glm::vec4(X[i], Y[i], height, 1.0f));
       surface.push_back(glm::vec4(X[i+1], Y[i+1], 0.0f, 1.0f));

       surface.push_back(glm::vec4(X[i+1], Y[i+1], 0.0f, 1.0f));
       surface.push_back(glm::vec4(X[i+1], Y[i+1], height, 1.0f));

       surface.push_back(glm::vec4(X[i], Y[i], height, 1.0f));
   }

   m_verticesSize = surface.size();
   m_vbo.updateBufferData(surface.data(), surface.size()*sizeof(glm::vec4));

}

void BSPLine::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }

}

void BSPLine::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }

}

void BSPLine::setColor(glm::vec4 color)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort colorLoc = glGetUniformLocation(m_program.getProgramID(), "u_color");
        glUniform4fv( colorLoc, 1, glm::value_ptr(color));
        _check_gl_error(__FILE__, __LINE__);
    }

}

void BSPLine::render()
{

    if (!m_initialized)
    {
        std::cerr << "Sphere.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();

//    glEnable(GL_CULL_FACE);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDrawElements(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr);
    _check_gl_error(__FILE__,__LINE__);
//    glDrawArraysInstanced(GL_TRIANGLES, 0, m_verticesSize);
    glDrawArrays(GL_TRIANGLES, 0, m_verticesSize);

    glDisable(GL_CULL_FACE);

    m_vao.unbind();
    m_program.release();

}
