#include <fstream>
#include <iostream>
#include <sstream>

#include "Model.h"

Model::Model(const std::string &fileName) :
    m_ibo(QOpenGLBuffer::IndexBuffer)
{
    loadObj(fileName);
    createOpenGLStuff();
}

void Model::loadObj(const std::string &fileName)
{
    std::ifstream input(fileName);
    std::string line;

    while (!input.eof())
    {
        std::getline(input, line);
        std::stringstream ss(line);

        if (line.empty() || line[0] == '#')
            continue;

        std::string command;
        ss >> command;

        if (command == "v")
        {
            float vec[3];
            ss >> vec[0] >> vec[1] >> vec[2];
            m_vertices.insert(m_vertices.end(), vec, vec+3);
        }
        else if (command == "f")
        {
            // TODO different formats!
            unsigned short indices[3];
            for (int i = 0; i < 3; i++)
            {
                ss >> indices[i];
                indices[i]--; // .obj files store 1 based indices!
            }
            m_indices.insert(m_indices.end(), indices, indices+3);
        }
        else
        {
            std::cout << "Ignoring '" << command << "' option in .obj file" << std::endl;
        }
    }
}

void Model::createOpenGLStuff()
{
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices.data(), m_vertices.size()*sizeof(float));

    // TODO for model or for shader?
    // Store the vertex attribute bindings for the program.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    m_vbo.release();

    // Create index buffer object.
    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(m_indices.data(), m_indices.size()*sizeof(short));
    m_ibo.release();
}

void Model::draw()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_ibo.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, NULL);
}
