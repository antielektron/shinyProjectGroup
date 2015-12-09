#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"

SceneEditorGame::SceneEditorGame() : 
		QObject(nullptr), 
		m_dummyCurrentObject(nullptr), 
		m_keyManager(nullptr)
{
}

void SceneEditorGame::initialize()
{
    m_scene.reset(new Scene());

    //create a Root object
    //(so far only managed by us)
    m_objectRoot.reset(new ObjectGroup());

    //such casting, much wow
    ObjectGroup *root = static_cast<ObjectGroup*>(m_objectRoot.get());

    root->setName("Root Object");

    m_scene->addModel("octo", std::unique_ptr<Model>(new Model("models/octonorm.obj")));
    m_dummyCurrentObject = m_scene->createObject("octo");

    //FIXME: this will result in a double free!
    root->addObject(m_dummyCurrentObject);

    m_scene->getCamera().translate(0., 0., -10);

    m_scene->setDirectionalLightDirection(QVector3D(0.0,0.0,-1.0));
    m_scene->setLightColor(QVector3D(1.0,1.0,1.0));

    emit currentObjectChanged();
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

void SceneEditorGame::currentObjectModified()
{
    m_dummyCurrentObject->updateWorld();
    emit currentObjectChanged();
}

void SceneEditorGame::getModels(std::vector<Model *> &models)
{
    // TODO
}


ObjectGroup *SceneEditorGame::getCurrentObjectGroup()
{
    // TODO
    return nullptr;
}

Object *SceneEditorGame::getCurrentObject()
{
    // TODO
    return m_dummyCurrentObject;
}
