#ifndef UNNAMED_PROJECT_ANIMATORSWIDGET_H
#define UNNAMED_PROJECT_ANIMATORSWIDGET_H

#include <QPushButton>
#include <QVBoxLayout>
#include "SceneEditor/AnimatorsTableWidget.h"
#include "GameLogic/GlobalState.h"
#include "GameLogic/Animator.h"
#include "Scene/Scene.h"

class AnimatorsWidget : public QWidget
{
    Q_OBJECT
public:
    AnimatorsWidget(QWidget *parent = nullptr);

    void setScene(Scene *scene);

public slots:
    void onAnimatorsChanged();

protected slots:
    void onAddAnimatorClicked();
    void onDelAnimatorClicked();

signals:
    void animatorAdded(std::unique_ptr<Animator> *animator);
    void animatorDeleted(Animator *anim);

protected:
    void generateWidgets();
    void connectStuff();

    AnimatorsTableWidget *m_animatorTable;

    QPushButton *m_addAnimator;
    QPushButton *m_delAnimator;

    QVBoxLayout *m_vLayout;

    Scene *m_scene;
};

#endif // UNNAMED_PROJECT_ANIMATORSWIDGET_H
