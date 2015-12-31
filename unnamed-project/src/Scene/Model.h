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

    float getRadius();
    const QVector3D &getCenter();
    const QVector3D &getMinExtent();
    const QVector3D &getMaxExtent();

    const std::vector<QVector3D> &getVertices();
    const std::vector<unsigned int> &getIndices();

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

    /**
     * update max sphere radius and bounding box
     */
    void updateExtent();

    // the models maximal radius
    float m_radius;

    // main emphasis
    QVector3D m_center;

    // rectiliniar extent:
    QVector3D m_minExtent;
    QVector3D m_maxExtent;

};

#endif // UNNAMED_PROJECT_SCENE_MODEL_H
