#ifndef UNNAMED_PROJECT_SCENE_MODEL_H
#define UNNAMED_PROJECT_SCENE_MODEL_H

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
    Model(const std::string &fileName, const std::string &name);

    void draw();
    std::string &getName();
    std::string &getFilename();

private:
    std::vector<QVector3D> m_vertices;
    std::vector<QVector3D> m_normals;
    std::vector<unsigned int> m_indices;

    std::string m_name;
    std::string m_filename; //remember, for loading and saving

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_indexBuffer;

    void loadObj(const std::string &fileName);
    void createOpenGLStuff();

    // get the models maximal extend
    float radius;

};

#endif // UNNAMED_PROJECT_SCENE_MODEL_H
