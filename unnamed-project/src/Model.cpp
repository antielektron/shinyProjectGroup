#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

#include "Model.h"

namespace std
{
    template<>
    struct less<std::pair<unsigned int, unsigned int>>
    {
        typedef std::pair<unsigned int, unsigned int> type;

        bool operator () (const type &lhs, const type &rhs)
        {
            if (lhs.first == rhs.first)
            {
                return lhs.second < rhs.second;
            }
            else
            {
                return lhs.first < rhs.first;
            }
        }
    };
}

Model::Model(const std::string &fileName) :
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    loadObj(fileName);
    createOpenGLStuff();
}

void Model::loadObj(const std::string &fileName)
{
    std::ifstream input(fileName);
    std::string line;

    std::vector<QVector3D> tempVertices;
    std::vector<QVector3D> tempNormals;

    // map from v, vt -> index
    std::map<std::pair<unsigned int, unsigned int>, unsigned int> peering;

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
            QVector3D vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            tempVertices.push_back(vec);
        }
        else if (command == "vn")
        {
            QVector3D vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            tempNormals.push_back(vec);
        }
        else if (command == "vt")
        {
        }
        else if (command == "f")
        {
            // TODO different formats!
            // NOTE: no quads!
            unsigned int indices[3];
            for (int i = 0; i < 3; i++)
            {
                unsigned int v;
                unsigned int vn;

                std::string entry;
                ss >> entry;

                size_t first = entry.find_first_of('/');
                size_t last = entry.find_last_of('/');

                assert(first != last);

                // .obj files store 1 based indices!
                v = std::stoi(entry) - 1;
                vn = std::stoi(entry.substr(last + 1)) - 1;

                auto it = peering.find({v, vn});
                if (it != peering.end())
                {
                    indices[i] = it->second;
                }
                else
                {
                    indices[i] = peering[{v, vn}] = m_vertices.size();
                    m_vertices.push_back(tempVertices[v]);
                    m_normals.push_back(tempNormals[vn]);
                }
            }

            m_indices.insert(m_indices.end(), indices, indices + 3);
        }
        else if (command == "s")
        {

        }
        else if (command == "g")
        {

        }
        else if (command == "usemtl")
        {

        }
        else
        {
            std::cout << "Ignoring '" << command << "' option in .obj file" << std::endl;
        }
    }
    std::cout << m_vertices.size() << std::endl;
}

void Model::createOpenGLStuff()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.data(), m_vertices.size()*3*sizeof(float));

    // Store the vertex attribute bindings for the program.
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    m_vertexBuffer.release();

    // Setup our normal buffer object.
    m_normalBuffer.create();
    m_normalBuffer.bind();
    m_normalBuffer.allocate(m_normals.data(), m_normals.size()*3*sizeof(float));

    // Store the vertex attribute bindings for the program.
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    m_normalBuffer.release();

    // Create index buffer object.
    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.data(), m_indices.size()*sizeof(int));
    m_indexBuffer.release();
}

void Model::draw()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL);
}
