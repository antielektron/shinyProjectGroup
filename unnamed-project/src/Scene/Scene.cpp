#include <cassert>
#include <iostream>
#include <stdexcept>

#include "Scene/Scene.h"

//------------------------------------------------------------------------------
Scene::Scene()
{
    m_rootGroup.setName("SceneRoot");
}

//------------------------------------------------------------------------------
Scene::Scene(const QString &filename)
{
    m_rootGroup.setName("SceneRoot");
    loadFromFile(filename);
}

//------------------------------------------------------------------------------
void Scene::clear()
{
    // TODO
}

//------------------------------------------------------------------------------
void Scene::loadFromFile(const QString &filename)
{
    // Good example for reading xml files with QT:
    // http://www.lucidarme.me/?p=2534

    // Open file
    QDomDocument document;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Could not open file '" + filename.toStdString() + "'");
    }
    document.setContent(&file);
    file.close();

    // Parse header
    QDomElement root = document.documentElement();
    QString rootType = root.tagName();              // should be 'Scene'
    m_sceneName = root.attribute("name", "unnamedScene");
    m_sceneVersion = root.attribute("version", "0.0");
    m_sceneAuthor = root.attribute("author", "Norby");

    // DEBUG
    std::cout << "Scene Info: " << std::endl
              << "\tname: " << m_sceneName.toStdString() << std::endl
              << "\tversion: " << m_sceneVersion.toStdString() << std::endl
              << "\tauthor: " << m_sceneAuthor.toStdString() << std::endl
              << std::endl;

    // Parse xml body
    for (auto currentElement = root.firstChildElement(); !currentElement.isNull(); currentElement = currentElement.nextSiblingElement())
    {
        QString tag = currentElement.tagName();
        std::cout << "TAG " << tag.toStdString() << std::endl;
        if (tag == "Objects")
        {
            readObjectTreeFromDom(&m_rootGroup, currentElement);
        }
        else if (tag == "Models")
        {
            readModelsFromDom(currentElement);
        }
        else if (tag == "DirectionalLight")
        {
            QVector3D pos = getPositionFromDom(currentElement);
            float r = currentElement.attribute("r", "0").toFloat();
            float g = currentElement.attribute("g", "0").toFloat();
            float b = currentElement.attribute("b", "0").toFloat();

            this->setDirectionalLightDirection(pos);
            this->setLightColor(QVector3D(r,g,b));

        }
        else if (tag == "Player")
        {
            // TODO: embed "Spawn" objects into the object tree, that the "real" game can query for!
            QVector3D pos = getPositionFromDom(currentElement);

            //TODO: set camera/player Position;
            std::cout << "TODO: set player Position to"
                      << " x = " << pos.x()
                      << ", y = " << pos.y()
                      << ", z = " << pos.z() << std::endl;
        }
        else
        {
            std::cout << "WARNING: unknown scene property '"
                      << tag.toStdString() << "'"
                      << std::endl;
        }
    }

    // Apply transformations to objects
    m_rootGroup.updateWorld();
}

//------------------------------------------------------------------------------
void Scene::readObjectTreeFromDom(ObjectGroup *root, const QDomElement &domElement)
{
    for (auto child = domElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        // TODO have `Object` and `ObjectGroup` children
        QString tag = child.tagName();

        if (tag == "Object")
        {
            QString name = child.attribute("name", "unnamedObject");
            QString modelName = child.attribute("model","");

            std::cout << name.toStdString() << " " << modelName.toStdString() << std::endl;

            auto object = createObject(modelName.toStdString(), root);
            object->setName(name);
            object->setPosition(getPositionFromDom(child));
            object->setRotation(getRotationFromDom(child));
            object->setScaling(getScalingFromDom(child));
        }
        else if(tag == "ObjectGroup")
        {
            QString name = child.attribute("name", "unnamedGroup");

            auto objectGroup = createObjectGroup(name.toStdString(), root);
            objectGroup->setPosition(getPositionFromDom(child));
            objectGroup->setRotation(getRotationFromDom(child));
            objectGroup->setScaling(getScalingFromDom(child));

            // Build object tree recursively
            readObjectTreeFromDom(objectGroup, child);
        }
        else
        {
            std::cout << "WARNING: Unexpected tag '"
                      << tag.toStdString()
                      << "' in Object Tree"
                      << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
void Scene::readModelsFromDom(const QDomElement &domElement)
{
    for (auto child = domElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        if (child.tagName() == "Model")
        {
            QString name = child.attribute("name","");
            QString path = child.attribute("path", "");

            //test if object has valid parameters:
            if (path.length() == 0)
            {
                std::cout << "WARNING: no filepath for model"
                << "' " << name.toStdString() << "' given"
                << std::endl;
            }
            else
            {
                if (name.length() > 0)
                {
                    Model *model = new Model(path.toStdString(), name.toStdString());
                    addModel(std::unique_ptr<Model>(model));
                }
                else
                {
                    // If no name is given it will be automatically
                    // generated by the Model consructor
                    Model *model = new Model(path.toStdString());
                    addModel(std::unique_ptr<Model>(model));
                }
            }
        }
        else
        {
            std::cout << "WARNING: Unexpected tag '"
            << child.tagName().toStdString()
            << "' in Model List"
            << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
QVector3D Scene::getPositionFromDom(const QDomElement &domElement)
{
    float x = domElement.attribute("x", "0").toFloat();
    float y = domElement.attribute("y", "0").toFloat();
    float z = domElement.attribute("z", "0").toFloat();
    return QVector3D(x, y, z);
}

//------------------------------------------------------------------------------
QVector3D Scene::getRotationFromDom(const QDomElement &domElement)
{
    float x = domElement.attribute("rx", "0").toFloat();
    float y = domElement.attribute("ry", "0").toFloat();
    float z = domElement.attribute("rz", "0").toFloat();
    return QVector3D(x, y, z);
}

//------------------------------------------------------------------------------
QVector3D Scene::getScalingFromDom(const QDomElement &domElement)
{
    float x = domElement.attribute("sx", "1").toFloat();
    float y = domElement.attribute("sy", "1").toFloat();
    float z = domElement.attribute("sz", "1").toFloat();
    return QVector3D(x, y, z);
}

//------------------------------------------------------------------------------
void Scene::saveToFile(const QString &filename)
{
    // Example: https://gist.github.com/lamprosg/2133804

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        std::cerr << "Could not write to file '" << filename.toStdString() << "'"
                  << std::endl;
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    // Write header (= root element)
    xmlWriter.writeStartElement("Scene");
    xmlWriter.writeAttribute("name", m_sceneName);
    xmlWriter.writeAttribute("version", m_sceneVersion);
    xmlWriter.writeAttribute("author", m_sceneAuthor);

    // Write models:
    xmlWriter.writeStartElement("Models");
    for (auto &mapItem : getModels())
    {
        Model *model = mapItem.second.get();
        xmlWriter.writeStartElement("Model");
        xmlWriter.writeAttribute("name", QString::fromStdString(model->getName()));
        xmlWriter.writeAttribute("path", QString::fromStdString(model->getFilename()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    // Write object tree:
    xmlWriter.writeStartElement("Objects");
    writeObjectTree(&m_rootGroup, xmlWriter);
    xmlWriter.writeEndElement();

    //and the little other properties:

    //light:
    xmlWriter.writeStartElement("DirectionalLight");
    writePosition(getDirectionalLightDirection(), xmlWriter);
    QVector3D lightColor = getLightColor();
    xmlWriter.writeAttribute("r", QString::number(lightColor.x()));
    xmlWriter.writeAttribute("g", QString::number(lightColor.y()));
    xmlWriter.writeAttribute("b", QString::number(lightColor.z()));
    xmlWriter.writeEndElement();

    //player:
    xmlWriter.writeStartElement("Player");
    writePosition(QVector3D(0,0,0), xmlWriter); //TODO
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    file.close();
}

//------------------------------------------------------------------------------
void Scene::writeObjectTree(ObjectGroup *root, QXmlStreamWriter &writer)
{
    // Write child groups:
    for (ObjectGroup *group : root->getGroups())
    {
        writer.writeStartElement("ObjectGroup");

        writer.writeAttribute("name", group->getName());

        writePosition(group->getPosition(), writer);
        writeRotation(group->getRotation(), writer);
        writeScaling(group->getScaling(), writer);

        // Write object tree recursively:
        writeObjectTree(group, writer);

        writer.writeEndElement();
    }

    // Write child objects:
    for (Object *object : root->getObjects())
    {
        QString modelName = QString::fromStdString(object->getModel()->getName());

        writer.writeStartElement("Object");

        writer.writeAttribute("name", object->getName());
        writer.writeAttribute("model", modelName);

        writePosition(object->getPosition(), writer);
        writeRotation(object->getRotation(), writer);
        writeScaling(object->getScaling(), writer);

        writer.writeEndElement();
    }
}

//------------------------------------------------------------------------------
void Scene::writePosition(const QVector3D &pos, QXmlStreamWriter &writer)
{
    writer.writeAttribute("x", QString::number(pos.x()));
    writer.writeAttribute("y", QString::number(pos.y()));
    writer.writeAttribute("z", QString::number(pos.z()));
}

//------------------------------------------------------------------------------
void Scene::writeRotation(const QVector3D &pos, QXmlStreamWriter &writer)
{
    writer.writeAttribute("rx", QString::number(pos.x()));
    writer.writeAttribute("ry", QString::number(pos.y()));
    writer.writeAttribute("rz", QString::number(pos.z()));
}

//------------------------------------------------------------------------------
void Scene::writeScaling(const QVector3D &pos, QXmlStreamWriter &writer)
{
    writer.writeAttribute("sx", QString::number(pos.x()));
    writer.writeAttribute("sy", QString::number(pos.y()));
    writer.writeAttribute("sz", QString::number(pos.z()));
}

//------------------------------------------------------------------------------
void Scene::setCamera(const QMatrix4x4 &camera)
{
    m_camera = camera;
}

//------------------------------------------------------------------------------
void Scene::setProjection(const QMatrix4x4 &proj)
{
    m_proj = proj;
}

//------------------------------------------------------------------------------
QMatrix4x4 &Scene::getCamera()
{
    return m_camera;
}

//------------------------------------------------------------------------------
QMatrix4x4 &Scene::getProjection()
{
    return m_proj;
}

//------------------------------------------------------------------------------
void Scene::addModel(std::unique_ptr<Model> model)
{
    assert(m_models.find(model->getName()) == m_models.end());
    m_models[model->getName()].swap(model);
}

//------------------------------------------------------------------------------
void Scene::removeModel(const std::string &modelName)
{
    assert(m_models.find(modelName) != m_models.end());
    m_models.erase(modelName);
}

//------------------------------------------------------------------------------
Object *Scene::createObject(const std::string &modelName, ObjectGroup *parent)
{
    if (!parent)
        parent = &m_rootGroup;

    assert(m_models.find(modelName) != m_models.end());
    Model *model = m_models[modelName].get();

    Object *object = new Object(model);
    object->setName(QString::fromStdString(modelName));

    m_objects.push_back(object);
    parent->addObject(std::unique_ptr<Object>(object));
    return object;
}

//------------------------------------------------------------------------------
ObjectGroup *Scene::createObjectGroup(const std::string &name, ObjectGroup *parent)
{
    if (!parent)
        parent = &m_rootGroup;

    ObjectGroup *newGroup = new ObjectGroup();
    newGroup->setName(QString::fromStdString(name));

    parent->addObjectGroup(std::unique_ptr<ObjectGroup>(newGroup));
    return newGroup;
}

//------------------------------------------------------------------------------
EditorObject *Scene::createEditorObject(const std::string &modelName)
{
    assert(m_models.find(modelName) != m_models.end());
    Model *model = m_models[modelName].get();

    EditorObject *object = new EditorObject(model);
    object->setName(QString::fromStdString(modelName));

    m_editorObjectRootGroup.addObject(
                std::unique_ptr<Object>(object));

    return object;
}

//------------------------------------------------------------------------------
range<Scene::ObjectIterator> Scene::getObjects()
{
    return range<ObjectIterator>(m_objects.cbegin(),m_objects.cend());
}

//------------------------------------------------------------------------------
range<ObjectGroup::object_iterator_type> Scene::getEditorObjects()
{
    return m_editorObjectRootGroup.getObjects();
}

//------------------------------------------------------------------------------
range<Scene::ModelIterator> Scene::getModels()
{
    return range<ModelIterator>(m_models.cbegin(), m_models.cend());
}

//------------------------------------------------------------------------------
QVector3D &Scene::getDirectionalLightDirection()
{
    return m_directionalLightDirection;
}

//------------------------------------------------------------------------------
QVector3D &Scene::getLightColor()
{
    return m_lightColor;
}

//------------------------------------------------------------------------------
void Scene::setLightColor(const QVector3D &color)
{
    this->m_lightColor = color;
}

//------------------------------------------------------------------------------
void Scene::setDirectionalLightDirection(const QVector3D &direction)
{
    this->m_directionalLightDirection = direction;
}

//------------------------------------------------------------------------------
ObjectGroup *Scene::getSceneRoot()
{
    return &m_rootGroup;
}

//------------------------------------------------------------------------------
void Scene::updateObjectList()
{
    m_objects.clear();
    ObjectGroup *root = &m_rootGroup;
    addToObjectList(root);
}

//------------------------------------------------------------------------------
void Scene::addToObjectList(ObjectGroup *root)
{
    for (Object *object : root->getObjects())
    {
        m_objects.push_back(object);
    }
    for (ObjectGroup *group : root->getGroups())
    {
        addToObjectList(group);
    }
}

//------------------------------------------------------------------------------
Model *Scene::getModel(const std::string &modelName)
{
    assert(m_models.find(modelName) != m_models.end());
    return m_models[modelName].get();
}

//------------------------------------------------------------------------------
void Scene::setName(const QString &name)
{
    m_sceneName = name;
}

//------------------------------------------------------------------------------
void Scene::setVersion(const QString &version)
{
    m_sceneVersion = version;
}

//------------------------------------------------------------------------------
void Scene::setAuthor(const QString &author)
{
    m_sceneAuthor = author;
}

//------------------------------------------------------------------------------
const QString &Scene::getName() const
{
    return m_sceneName;
}

//------------------------------------------------------------------------------
const QString &Scene::getVersion() const
{
    return m_sceneVersion;
}

//------------------------------------------------------------------------------
const QString &Scene::getAuthor() const
{
    return m_sceneAuthor;
}
