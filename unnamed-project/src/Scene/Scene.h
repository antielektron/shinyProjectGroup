#ifndef UNNAMED_PROJECT_SCENE_SCENE_H
#define UNNAMED_PROJECT_SCENE_SCENE_H

#include <QMatrix4x4>
#include <QColor>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <utility>

//for import-/export functions
#include <QtXml/QDomDocument>
#include <QFile>
#include <QXmlStreamWriter>

#include "Scene/Model.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"
#include "smartiterator.h"

class Scene
{
public:

    Scene();

    /**
     * SceneInfo is defined as a tuple containing the scene's
     * name, version and author as a QString
     */
    typedef std::tuple<QString, QString, QString> SceneInfo;

    static std::pair<Scene::SceneInfo, Scene *> loadFromFile(const QString &filename);
    static void saveToFile(Scene *scene, const QString &filename, SceneInfo &info);

    void setCamera(const QMatrix4x4 &camera);
    void setProjection(const QMatrix4x4 &proj);

    void setLightColor(const QVector3D &color);
    void setDirectionalLightDirection(const QVector3D &direction);

    ObjectGroup *getSceneRoot();

    typedef std::map<std::string, std::unique_ptr<Model>>::const_iterator ModelIterator;
    range<ModelIterator> getModels();

    QMatrix4x4 &getCamera();
    QMatrix4x4 &getProjection();
    QVector3D &getDirectionalLightDirection();
    QVector3D &getLightColor();

    void addModel(std::unique_ptr<Model> model);
    Object *createObject(const std::string &modelName, ObjectGroup *parent = nullptr);

    ObjectGroup *createObjectGroup(const std::string &name, ObjectGroup *parent = nullptr);

    typedef std::vector<Object *>::const_iterator ObjectIterator;
    range<ObjectIterator> getObjects();

    Model *getModel(const std::string &modelName);

private:
    static void readObjects(Scene *scene, ObjectGroup *root,QDomElement *domElem);
    static QVector3D getPositionFromDomElement(const QDomElement &elem);
    static void readModels(Scene *scene, QDomElement *domElem);
    static void writeObjectTree(ObjectGroup *objectgroup,QXmlStreamWriter &writer);
    static void writePositionToDomElement(QXmlStreamWriter &writer,const QVector3D &pos);

    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    /// QMatrix4x4 m_world;

    QVector3D m_directionalLightDirection;
    QVector3D m_lightColor;

    std::unique_ptr<ObjectGroup> m_rootGroup;

    //TODO: get names directly from objects
    //(is more efficient than ask the scene
    // every time for it! )
    // Model <- SceneModel <- SceneObject hm
    std::map<std::string, std::unique_ptr<Model>> m_models; // all basic models that are available (only for construction purposes, if needed!)

    std::vector<Object *> m_objects;
};

#endif // UNNAMED_PROJECT_SCENE_SCENE_H
