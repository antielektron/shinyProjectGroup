#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"

SceneEditorGame::SceneEditorGame() : 
		QObject(nullptr), 
		m_dummyCurrentObject(nullptr), 
        m_keyManager(nullptr),
        m_initialized(false)
{
}

void SceneEditorGame::initialize()
{
    //load a test level by default:
    initialize(std::unique_ptr<Scene>(Scene::loadFromFile("level/test.xml").second));
}

void SceneEditorGame::initialize(std::unique_ptr<Scene> scene)
{
    m_scene = std::move(scene);

    m_initialized = true;

    m_dummyCurrentObject = nullptr;

    m_scene->getCamera().translate(0., 0., -10);

    m_scene->getSceneRoot()->updateWorld();

    emit currentObjectChanged();
    emit modelsChanged();
    emit objectsChanged();
    emit sceneReloaded();
}

void SceneEditorGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void SceneEditorGame::tick()
{
	QVector3D deltaPos(0, 0, 0);

	if (!m_keyManager)
		return;

	m_keyManager->tick();

	if (m_keyManager->shouldCatchMouse())
	{
		rotX += m_keyManager->getRelativeY() * .1;
		rotY += m_keyManager->getRelativeX() * .1;
	}

	if (m_keyManager->isKeyPressed(Qt::Key_Right))
	{
		rotY -= 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Left))
	{
		rotY += 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Up))
	{
		rotX += 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Down))
	{
		rotX -= 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_S))
	{
		deltaPos += QVector3D(0, 0, 0.5);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_W))
	{
		deltaPos += QVector3D(0, 0, -0.5);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_D))
	{
		deltaPos += QVector3D(0.5, 0, 0);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_A))
	{
		deltaPos += QVector3D(-0.5, 0, 0);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Q))
	{
		deltaPos += QVector3D(0, 0.5, 0);
	}
	// !!! Keyboard layout
	if (m_keyManager->isKeyPressed(Qt::Key_Z))
	{
		deltaPos += QVector3D(0, -0.5, 0);
	}

	// Reset camera
	if (m_keyManager->isKeyPressed(Qt::Key_Space))
	{
		m_position = QVector3D(0, 0, 0);
		rotX = 0;
		rotY = 0;
	}

	if (rotX > 90.f)
		rotX = 90.f;
	if (rotX < -90.f)
		rotX = -90.f;

	updatePosMatrix(deltaPos);

	// Start/Stop catching mouse
	if (m_keyManager->isKeyPressed(Qt::Key_Escape) && !m_wasEscDown)
	{
		m_keyManager->setCatchMouse(!m_keyManager->shouldCatchMouse());
	}
	m_wasEscDown = m_keyManager->isKeyPressed(Qt::Key_Escape);
}

Scene *SceneEditorGame::getScene()
{
    return m_scene.get();
}

void SceneEditorGame::setKeyManager(KeyManager *keyManager)
{
	this->m_keyManager = keyManager;
}

bool SceneEditorGame::isInitialized()
{
    return m_initialized;
}

ObjectGroup *SceneEditorGame::getRootObject()
{
    return m_scene->getSceneRoot();
}

void SceneEditorGame::updatePosMatrix(QVector3D deltaPos)
{
	QMatrix4x4 &camera = m_scene->getCamera();
	QMatrix4x4 translation;
	QMatrix4x4 xRotation;
	QMatrix4x4 yRotation;

	xRotation.rotate(rotX, 1, 0, 0);
	yRotation.rotate(rotY, 0, 1, 0);
	translation.translate(-1 * m_position);

	QMatrix4x4 rotation = xRotation * yRotation;

	m_position += rotation.transposed() * deltaPos;

	camera = rotation * translation;
}

void SceneEditorGame::currentObjectModified(ObjectBase* object)
{
    m_dummyCurrentObject = object;
    m_dummyCurrentObject->updateWorld();
    emit currentObjectChanged();
}

void SceneEditorGame::getModels(std::vector<Model *> &models)
{
    for (auto &mapItem : m_scene->getModels())
    {
        models.push_back(mapItem.second.get());
    }
}

void SceneEditorGame::addModel(std::unique_ptr<Model> model)
{
    m_scene->addModel(std::move(model));
    emit modelsChanged();
}

Object *SceneEditorGame::createObject(const std::string &name)
{
    Object *obj = m_scene->createObject(name);
    emit objectsChanged();
    return obj;
}

ObjectGroup *SceneEditorGame::createObjectGroup(const std::string &name, ObjectGroup *parent)
{
    ObjectGroup *objGrp = m_scene->createObjectGroup(name, parent);
    emit objectsChanged();
    return objGrp;
}

