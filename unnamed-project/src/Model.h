#ifndef UNNAMED_PROJECT_MODEL_H
#define UNNAMED_PROJECT_MODEL_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include <string>
#include <vector>

class Model
{
public:
    Model(const std::string &fileName);

    void draw();

private:
    std::vector<float> m_vertices;
    std::vector<unsigned short> m_indices;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;

    void loadObj(const std::string &fileName);
    void createOpenGLStuff();

};

#endif // UNNAMED_PROJECT_MODEL_H
