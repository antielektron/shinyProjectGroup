#ifndef UNNAMED_PROJECT_PRIMITIVE_GAME_H
#define UNNAMED_PROJECT_PRIMITIVE_GAME_H

#include <QMatrix4x4>
#include "KeyManager.h"

#include "IGame.h"
#include "Scene/Scene.h"

class PrimitiveGame : public IGame
{
public:
	PrimitiveGame();

    virtual void initialize() override;
    virtual void resize(int width, int height) override;
    virtual void tick(float dt = 1.0f/60.0f) override;

    virtual Scene *getScene() override;

private:
    std::unique_ptr<Scene> m_scene;

	float rotY = 0;
	float rotX = 0;
	float pos_height = 0;
	

	QVector3D m_position;
	QVector3D m_centre;
	QMatrix4x4 posMatrix;

	bool m_wasEscDown;

	void updatePosMatrix(QVector3D deltaPos);
};

#endif // UNNAMED_PROJECT_PRIMITIVE_GAME_H
