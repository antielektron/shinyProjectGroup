#ifndef UNNAMED_PROJECT_MODEL_H
#define UNNAMED_PROJECT_MODEL_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include <QVector3D>

#include <string>
#include <vector>

class Model
{
public:
    Model(const std::string &fileName);

    void draw();

private:
    std::vector<QVector3D> m_vertices;
    std::vector<QVector3D> m_normals;
    std::vector<unsigned int> m_indices;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_indexBuffer;

    void loadObj(const std::string &fileName);
    void createOpenGLStuff();

};

#endif // UNNAMED_PROJECT_MODEL_H
