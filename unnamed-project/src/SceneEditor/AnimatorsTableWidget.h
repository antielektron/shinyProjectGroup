#ifndef UNNAMED_PROJECT_ANIMATORSTABLEWIDGET_H
#define UNNAMED_PROJECT_ANIMATORSTABLEWIDGET_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <vector>
#include "GameLogic/Animator.h"

class Scene;

class AnimatorsTableWidget : public QTableView
{
    Q_OBJECT
public:
    AnimatorsTableWidget(QWidget *parent = nullptr);

    void init();

    void addItem(const QString &object,
                 const QString &animatorType,
                 const QString &watchedAttrib,
                 const QString &interpolationType,
                 const QString &duration);

    Animator *getAnimator(size_t index);

public slots:
    void onEventsChanged(Scene *scene);

protected:
    void rebuildList(Scene *scene);

    QStandardItemModel *m_model;
    std::vector<Animator *> m_animatorList;
};

#endif // UNNAMED_PROJECT_ANIMATORSTABLEWIDGET_H
