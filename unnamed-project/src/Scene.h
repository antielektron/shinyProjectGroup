#ifndef UNNAMED_PROJECT_SCENE_H
#define UNNAMED_PROJECT_SCENE_H

#include <QMatrix4x4>
#include <QColor>

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "Model.h"
#include "Object.h"

class Scene
{
public:
    // TODO

    void setCamera(const QMatrix4x4 &camera);
    void setProjection(const QMatrix4x4 &proj);

    void setLightColor(const QVector3D &color);
    void setDirectionalLightDirection(const QVector3D &direction);

    QMatrix4x4 &getCamera();
    QMatrix4x4 &getProjection();
    QVector3D &getDirectionalLightDirection();
    QVector3D &getLightColor();

    void addModel(const std::string &name, std::unique_ptr<Model> model);
    Object *createObject(const std::string &modelName);

    typedef std::vector<std::unique_ptr<Object>>::const_iterator ObjectIterator;
    ObjectIterator objectsBegin();
    ObjectIterator objectsEnd();

private:
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    /// QMatrix4x4 m_world;

    QVector3D m_directionalLightDirection;
    QVector3D m_lightColor;

    // Model <- SceneModel <- SceneObject hm
    std::map<std::string, std::unique_ptr<Model>> m_models; // all basic models that are available (only for construction purposes, if needed!)

    std::vector<std::unique_ptr<Object>> m_objects;
};

#endif // UNNAMED_PROJECT_SCENE_H
