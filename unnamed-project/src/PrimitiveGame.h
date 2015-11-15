#ifndef UNNAMED_PROJECT_PRIMITIVE_GAME_H
#define UNNAMED_PROJECT_PRIMITIVE_GAME_H

#include "IGame.h"
#include "Scene.h"
#include <QMatrix4x4>

class PrimitiveGame : public IGame
{
public:
    virtual void initialize() override;
    virtual void resize(int width, int height) override;
    virtual void tick() override;

    virtual Scene *getScene() override;

	virtual void onDoubleClick() override;

	virtual void onKeyEvent(int key) override;

private:
    std::unique_ptr<Scene> m_scene;

    Object *m_dummy;
    float r;
	float rotY = 0;
	float rotX = 0;

	QVector3D m_position;
	QVector3D m_centre;
	

};

#endif // UNNAMED_PROJECT_PRIMITIVE_GAME_H
