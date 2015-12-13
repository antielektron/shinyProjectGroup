#include <cassert>
#include <iostream>

#include "Scene/Scene.h"

//------------------------------------------------------------------------------
Scene::Scene()
{
    ObjectGroup *rootGroup = new ObjectGroup();
    rootGroup->setName("SceneRoot");
    m_rootGroup.reset(rootGroup);
}

//------------------------------------------------------------------------------
std::pair<Scene::SceneInfo, Scene *> Scene::loadFromFile(const QString &filename)
{
    //good example for reading xml files with QT:
    //http://www.lucidarme.me/?p=2534

    //open file:
    QDomDocument document;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Could not open file '" << filename.toStdString() << "'"
                  << std::endl;

        SceneInfo dummyInfo = std::make_tuple("","","");
        return std::make_pair(dummyInfo, nullptr);
    }
    document.setContent(&file);
    file.close();

    //parse header
    QDomElement root = document.documentElement();
    QString rootType = root.tagName();              //should be 'Scene'
    QString sceneName = root.attribute("name", "unnamedScene");
    QString sceneVersion = root.attribute("version", "0.0");
    QString sceneAuthor = root.attribute("author", "Norby");

    SceneInfo info = std::make_tuple(sceneName, sceneVersion, sceneAuthor);

    //DEBUG
    std::cout << "Scene Info: "
              << "\tname: " << sceneName.toStdString()
              << "\tversion: " << sceneVersion.toStdString()
              << "\tsceneAuthor " << sceneAuthor.toStdString()
              << std::endl;

    //create Scene
    Scene *scene = new Scene();

    //parse xml body
    QDomElement currentElement = root.firstChildElement();

    while (!currentElement.isNull())
    {
        QString tag = currentElement.tagName();

        if (tag == "Objects")
        {
            Scene::readObjects(scene, nullptr, &currentElement);
        }
        else if (tag == "Models")
        {
            Scene::readModels(scene, &currentElement);
        }
        else if (tag == "DirectionalLight")
        {
            QVector3D pos = Scene::getPositionFromDomElement(currentElement);
            float r = currentElement.attribute("r", "0").toFloat();
            float g = currentElement.attribute("g", "0").toFloat();
            float b = currentElement.attribute("b", "0").toFloat();

            scene->setDirectionalLightDirection(pos);
            scene->setLightColor(QVector3D(r,g,b));

        }
        else if (tag == "Player")
        {
            QVector3D pos = Scene::getPositionFromDomElement(currentElement);

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


        //get next dom element
        currentElement = currentElement.nextSiblingElement();
    }

    return std::make_pair(info, scene);
}

//------------------------------------------------------------------------------
void Scene::readObjects(Scene *scene, ObjectGroup *root, QDomElement *domElem)
{
    QDomElement child = domElem->firstChildElement();

    while(!child.isNull())
    {
        QString tag = child.tagName();
        if (child.tagName() == "Object")
        {
            QString attr = child.attribute("type","notype");

            if (attr == "notype")
            {
                std::cout << "WARNING: empty object '"
                          << child.attribute("name", "unnamed").toStdString()
                          << "'" << std::endl;
            }
            else if (attr == "Object")
            {
                QString name = child.attribute("name", "unnamedObject");
                QVector3D pos = Scene::getPositionFromDomElement(child);

                //getModel
                QString modelName = child.attribute("model","");
                Object *obj = scene->createObject(modelName.toStdString(), root);
                obj->setName(name);
                obj->setPosition(pos);
            }
            else if(attr == "ObjectGroup")
            {
                QString name = child.attribute("name", "unnamedGroup");
                QVector3D pos = Scene::getPositionFromDomElement(child);
                ObjectGroup *objGrp = scene->createObjectGroup(name.toStdString(),root);
                objGrp->setPosition(pos);

                QDomElement objGrpChildDomElem = child.firstChildElement();

                //build object tree recursively
                readObjects(scene, objGrp, &objGrpChildDomElem);

            }
            else
            {
                std::cout << "WARNING: unkown objectType '"
                          << attr.toStdString()
                          << "' " << std::endl;
            }

        }
        else{
            std::cout << "WARNING: Unexpected tag '"
                      << tag.toStdString()
                      << "' in Object Tree"
                      << std::endl;
        }
        child = child.nextSiblingElement();
    }
}

//------------------------------------------------------------------------------
QVector3D Scene::getPositionFromDomElement(const QDomElement &elem)
{
    float x = elem.attribute("x", "0").toFloat();
    float y = elem.attribute("y", "0").toFloat();
    float z = elem.attribute("z", "0").toFloat();
    return QVector3D(x, y, z);
}

//------------------------------------------------------------------------------
void Scene::readModels(Scene *scene, QDomElement *domElem)
{
    QDomElement child = domElem->firstChildElement();

    while(!child.isNull())
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
                    scene->addModel(std::unique_ptr<Model>(model));
                }
                else
                {
                    //if no name is given it will be automatically
                    //generated by the Model consructor
                    Model *model = new Model(path.toStdString());
                    scene->addModel(std::unique_ptr<Model>(model));
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

        //get next dom element:
        child = child.nextSiblingElement();
    }
}

//------------------------------------------------------------------------------
void Scene::saveToFile(Scene *scene, const QString &filename, SceneInfo &info)
{
    //example: https://gist.github.com/lamprosg/2133804
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

    //write header (= root element)
    xmlWriter.writeStartElement("Scene");
    xmlWriter.writeAttribute("name",std::get<0>(info));
    xmlWriter.writeAttribute("version", std::get<1>(info));
    xmlWriter.writeAttribute("author", std::get<2>(info));

    //write models:
    xmlWriter.writeStartElement("Models");
    for (auto &mapItem : scene->getModels())
    {
        Model *model = mapItem.second.get();
        xmlWriter.writeStartElement("Model");
        xmlWriter.writeAttribute("name", QString::fromStdString( model->getName()));
        xmlWriter.writeAttribute("path", QString::fromStdString(model->getFilename()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    //write object tree:
    xmlWriter.writeStartElement("Objects");
    ObjectGroup *root = scene->getSceneRoot();
    Scene::writeObjectTree(root, xmlWriter);
    xmlWriter.writeEndElement();

    //and the little other properties:

    //light:
    xmlWriter.writeStartElement("DirectionalLight");
    Scene::writePositionToDomElement(xmlWriter, scene->getDirectionalLightDirection());
    QVector3D lightColor = scene->getLightColor();
    xmlWriter.writeAttribute("r", QString::number(lightColor.x()));
    xmlWriter.writeAttribute("g", QString::number(lightColor.y()));
    xmlWriter.writeAttribute("b", QString::number(lightColor.z()));
    xmlWriter.writeEndElement();

    //player:
    xmlWriter.writeStartElement("Player");
    Scene::writePositionToDomElement(xmlWriter,QVector3D(0,0,0)); //TODO
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    file.close();
}

//------------------------------------------------------------------------------
void Scene::writeObjectTree(ObjectGroup *objectgroup, QXmlStreamWriter &writer)
{
    //write child groups:
    for (ObjectGroup *group : objectgroup->getGroups())
    {
        QString name = group->getName();
        QVector3D pos = group->getPosition();
        writer.writeStartElement("Object");
        writer.writeAttribute("type","ObjectGroup");
        writer.writeAttribute("name",name);
        Scene::writePositionToDomElement(writer, pos);

        //write object tree recursively:
        Scene::writeObjectTree(group, writer);

        writer.writeEndElement();
    }

    //write child Objects:
    for (Object *object : objectgroup->getObjects())
    {
        QString name = object->getName();
        QVector3D pos = object->getPosition();
        QString modelName = QString::fromStdString(object->getModel()->getName());
        writer.writeStartElement("Object");
        writer.writeAttribute("type","Object");
        writer.writeAttribute("name",name);
        writer.writeAttribute("model",modelName);
        Scene::writePositionToDomElement(writer, pos);
        writer.writeEndElement();
    }
}

//------------------------------------------------------------------------------
void Scene::writePositionToDomElement(QXmlStreamWriter &writer, const QVector3D &pos)
{
    writer.writeAttribute("x", QString::number(pos.x()));
    writer.writeAttribute("y", QString::number(pos.y()));
    writer.writeAttribute("z", QString::number(pos.z()));
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
Object *Scene::createObject(const std::string &modelName, ObjectGroup *parent)
{
    if (!parent)
    {
        parent = m_rootGroup.get();
    }

    assert(m_models.find(modelName) != m_models.end());
    Model *model = m_models[modelName].get();
    Object *object = new Object(model);
    object->setName(QString::fromStdString(modelName)); //TODO: discuss how to choose identifiers!
    m_objects.push_back(object); // construct unique_ptr automatically
    parent->addObject(std::unique_ptr<Object>(object));
    return object;
}

//------------------------------------------------------------------------------
ObjectGroup *Scene::createObjectGroup(const std::string &name, ObjectGroup *parent)
{
    if (!parent)
    {
        parent = m_rootGroup.get();
    }

    ObjectGroup *newGroup = new ObjectGroup();
    newGroup->setName(QString::fromStdString(name));
    parent->addObjectGroup(std::unique_ptr<ObjectGroup>(newGroup));

    return newGroup;
}

//------------------------------------------------------------------------------
range<Scene::ObjectIterator> Scene::getObjects()
{
    return range<ObjectIterator>(m_objects.cbegin(),m_objects.cend());
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
    return m_rootGroup.get();
}

//------------------------------------------------------------------------------
Model *Scene::getModel(const std::string &modelName)
{
    assert(m_models.find(modelName) != m_models.end());
    return m_models[modelName].get();
}



