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

#include "SceneEditor/EditorObject.h"

class Scene
{
public:
    Scene();
    Scene(const QString &filename);

    void clear();
    void loadFromFile(const QString &filename);
    void saveToFile(const QString &filename);

    void setCamera(const QMatrix4x4 &camera);
    void setProjection(const QMatrix4x4 &proj);

    void setLightColor(const QVector3D &color);
    void setDirectionalLightDirection(const QVector3D &direction);

    QMatrix4x4 &getCamera();
    QMatrix4x4 &getProjection();

    QVector3D &getDirectionalLightDirection();
    QVector3D &getLightColor();

    void addModel(std::unique_ptr<Model> model);
    void removeModel(const std::string &modelName);

    Object *createObject(const std::string &modelName, ObjectGroup *parent = nullptr);
    ObjectGroup *createObjectGroup(const std::string &name, ObjectGroup *parent = nullptr);

    EditorObject *createEditorObject(const std::string &modelName);

    ObjectGroup *getSceneRoot();

    void updateObjectList();

    typedef std::map<std::string, std::unique_ptr<Model>>::const_iterator ModelIterator;
    range<ModelIterator> getModels();

    typedef std::vector<Object *>::const_iterator ObjectIterator;
    range<ObjectIterator> getObjects();

    range<ObjectGroup::object_iterator_type> getEditorObjects();

    Model *getModel(const std::string &modelName);


    // Access metadata
    void setName(const QString &name);
    void setVersion(const QString &version);
    void setAuthor(const QString &author);

    const QString &getName() const;
    const QString &getVersion() const;
    const QString &getAuthor() const;

private:
    void readObjectTreeFromDom(ObjectGroup *root, const QDomElement &domElement);
    void readModelsFromDom(const QDomElement &domElem);
    QVector3D getPositionFromDom(const QDomElement &domElement);
    QVector3D getRotationFromDom(const QDomElement &domElement);
    QVector3D getScalingFromDom(const QDomElement &domElement);

    void writeObjectTree(ObjectGroup *root, QXmlStreamWriter &writer);
    void writeModels(QXmlStreamWriter &writer);
    void writePosition(const QVector3D &position, QXmlStreamWriter &writer);
    void writeRotation(const QVector3D &rotation, QXmlStreamWriter &writer);
    void writeScaling(const QVector3D &scaling, QXmlStreamWriter &writer);

    void addToObjectList(ObjectGroup *group);

    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    /// QMatrix4x4 m_world;

    QVector3D m_directionalLightDirection;
    QVector3D m_lightColor;

    // No pointer: force to exist.
    ObjectGroup m_rootGroup;

    // Root for Editor- (aka Debug-) objects.
    // Scene only allows to add objects to that root,
    // groups are not allowed as child
    ObjectGroup m_editorObjectRootGroup;

    //TODO: get names directly from objects
    //(is more efficient than ask the scene
    // every time for it! )
    // Model <- SceneModel <- SceneObject hm
    std::map<std::string, std::unique_ptr<Model>> m_models; // all basic models that are available (only for construction purposes, if needed!)

    std::vector<Object *> m_objects;

    // Meta information
    QString m_sceneName;
    QString m_sceneVersion;
    QString m_sceneAuthor;
};

#endif // UNNAMED_PROJECT_SCENE_SCENE_H
