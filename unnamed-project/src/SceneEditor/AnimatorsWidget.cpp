#include "SceneEditor/AnimatorsWidget.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>

#include "GameLogic/PositionAnimator.h"
#include "GameLogic/RotationAnimator.h"

//------------------------------------------------------------------------------
AnimatorsWidget::AnimatorsWidget(QWidget *parent) : QWidget (parent)
{
    setScene(nullptr);
    generateWidgets();
    connectStuff();
}

//------------------------------------------------------------------------------
void AnimatorsWidget::setScene(Scene *scene)
{
    m_scene = scene;
}

//------------------------------------------------------------------------------
void AnimatorsWidget::generateWidgets()
{
    m_vLayout = new QVBoxLayout(this);

    m_animatorTable = new AnimatorsTableWidget(this);
    m_animatorTable->setMinimumSize(200,100);

    m_addAnimator = new QPushButton("add animator", this);
    m_delAnimator = new QPushButton("del animator", this);

    m_vLayout->addWidget(m_animatorTable);

    m_vLayout->addWidget(m_addAnimator);
    m_vLayout->addWidget(m_delAnimator);

    this->setLayout(m_vLayout);
}

//------------------------------------------------------------------------------
void AnimatorsWidget::connectStuff()
{
    connect(m_addAnimator, SIGNAL(clicked()), this, SLOT(onAddAnimatorClicked()));
    connect(m_delAnimator, SIGNAL(clicked()), this, SLOT(onDelAnimatorClicked()));
}

//------------------------------------------------------------------------------
void AnimatorsWidget::onAnimatorsChanged()
{
    if (m_scene)
    {
        m_animatorTable->onEventsChanged(m_scene);
    }
}

//------------------------------------------------------------------------------
void AnimatorsWidget::onAddAnimatorClicked()
{
    if (!m_scene)
    {
        QMessageBox::warning(this, "Warning", "Scene not initialized yet");
        return;
    }

    //prepare Lists:
    QStringList objects; // does not include objGroups, but user can type an
                         // objGroup key manually
    QStringList attribs;
    QStringList animationTypes;
    QStringList interpolationTypes;

    for (const auto &obj : m_scene->getObjects())
    {
        objects.append(obj->getName());
    }

    for (const auto &attrib : m_scene->getGlobalState()->getAttributes())
    {
        attribs.append(attrib.first);
    }

    for (const auto &animType : qStringToAnimation)
    {
        animationTypes.append(animType.first);
    }

    for (const auto &interpolationType : qStringToInterpolation)
    {
        interpolationTypes.append(interpolationType.first);
    }

    bool ok;
    QString objectKey = QInputDialog::getItem(this,
                                              "select object key",
                                              "key",
                                              objects,
                                              0,
                                              true,
                                              &ok);
    if (!ok)
    {
        return;
    }

    QString watchedAttrib = QInputDialog::getItem(this,
                                                  "select attribute key",
                                                  "key",
                                                  attribs,
                                                  0,
                                                  false,
                                                  &ok);
    if (!ok)
    {
        return;
    }

    QString animType = QInputDialog::getItem(this,
                                             "select animation type",
                                             "type",
                                             animationTypes,
                                             0,
                                             false,
                                             &ok);
    if (!ok)
    {
        return;
    }

    QString interpolationType = QInputDialog::getItem(this,
                                                      "select interpolation type",
                                                      "type",
                                                      interpolationTypes,
                                                      0,
                                                      false,
                                                      &ok);
    if (!ok)
    {
        return;
    }

    float animTime = static_cast<float>(QInputDialog::getDouble(this,
                                             "enter Animation Time",
                                             "time in seconds",
                                             1.0,
                                             0.0,
                                             10000.0,
                                             3,
                                             &ok));
    if (!ok)
    {
        return;
    }

    ObjectBase *obj = m_scene->findObjectByName(m_scene->getSceneRoot(),
                                                objectKey);

    if (!obj)
    {
        QMessageBox::warning(this, "Warning", "invalid Object Key");
        return;
    }

    Animator *animator = nullptr;

    switch(qStringToAnimation.at(animType))
    {
    case AnimationType::Position:
    {
        animator = new PositionAnimator(m_scene->findObjectByName(
                                            m_scene->getSceneRoot(), objectKey),
                                        m_scene->getGlobalState(),
                                        watchedAttrib,
                                        qStringToInterpolation.at(interpolationType),
                                        animTime);
        break;
    }
    case AnimationType::Rotation:
    {
        animator = new RotationAnimator(m_scene->findObjectByName(
                                            m_scene->getSceneRoot(), objectKey),
                                        m_scene->getGlobalState(),
                                        watchedAttrib,
                                        qStringToInterpolation.at(interpolationType),
                                        animTime);
        break;
    }
    case AnimationType::Scaling:
    {
        QMessageBox::warning(this, "Warning", "Scaling not supported yet");
        return;
    }
    }

    std::unique_ptr<Animator> animatorPtr(animator);

    emit animatorAdded(&animatorPtr);
}

//------------------------------------------------------------------------------
void AnimatorsWidget::onDelAnimatorClicked()
{
    QModelIndexList selectedList = m_animatorTable->selectionModel()->selectedRows();
    for (int i = 0; i < selectedList.size(); i++)
    {
        emit animatorDeleted(m_animatorTable->getAnimator(selectedList.at(i).row()));
    }
}
