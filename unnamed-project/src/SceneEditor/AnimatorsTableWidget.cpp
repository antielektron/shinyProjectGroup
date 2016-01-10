#include "SceneEditor/AnimatorsTableWidget.h"

#include <QStandardItem>
#include <QString>
#include <QList>

#include "GameLogic/GameLogicDatatypes.h"
#include "Scene/Scene.h"

//------------------------------------------------------------------------------
AnimatorsTableWidget::AnimatorsTableWidget(QWidget *parent) :
    QTableView(parent)
{
    init();
}

//------------------------------------------------------------------------------
void AnimatorsTableWidget::init()
{
    m_model = new QStandardItemModel(0, 4, this);

    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("object")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("animator type")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("watched attribute")));
    m_model->setHorizontalHeaderItem(3, new QStandardItem(QString("interpolation")));
    m_model->setHorizontalHeaderItem(4, new QStandardItem(QString("duration")));

    setModel(m_model);

    setShowGrid(false);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//------------------------------------------------------------------------------
void AnimatorsTableWidget::addItem(const QString &object,
                                   const QString &animatorType,
                                   const QString &watchedAttrib,
                                   const QString &interpolationType,
                                   const QString &duration)
{
    QList<QStandardItem *> list;

    list.append(new QStandardItem(object));
    list.append(new QStandardItem(animatorType));
    list.append(new QStandardItem(watchedAttrib));
    list.append(new QStandardItem(interpolationType));
    list.append(new QStandardItem(duration));

    m_model->appendRow(list);
}

//------------------------------------------------------------------------------
Animator *AnimatorsTableWidget::getAnimator(size_t index)
{
    return m_animatorList[index];
}

//------------------------------------------------------------------------------
void AnimatorsTableWidget::onEventsChanged(Scene *scene)
{
    rebuildList(scene);
}

//------------------------------------------------------------------------------
void AnimatorsTableWidget::rebuildList(Scene *scene)
{
    m_model->clear();
    m_animatorList.clear();

    m_model->setRowCount(0);
    m_model->setColumnCount(3);

    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("object")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("animator type")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("watched attribute")));
    m_model->setHorizontalHeaderItem(3, new QStandardItem(QString("interpolation")));
    m_model->setHorizontalHeaderItem(4, new QStandardItem(QString("duration")));

    for (const auto &animator : scene->getAnimators())
    {
        QString object = animator->getObject()->getName();
        QString animatorType = animationToQString.at(animator->getAnimationType());
        QString watchedAttrib = animator->getAttributeKey();
        QString interpolationType = interpolationToQString.at(
                                        animator->getInterpolationType());
        QString duration = QString::number(animator->getAnimationTime());

        addItem(object, animatorType, watchedAttrib, interpolationType, duration);

        m_animatorList.push_back(animator.get());
    }
}
