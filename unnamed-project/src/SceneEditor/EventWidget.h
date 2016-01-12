#ifndef UNNAMED_PROJECT_EVENTWIDGET_H
#define UNNAMED_PROJECT_EVENTWIDGET_H

#include <QPushButton>
#include <QVBoxLayout>
#include "SceneEditor/EventTableWidget.h"
#include "GameLogic/GlobalState.h"

class Scene;

class EventWidget : public QWidget
{
    Q_OBJECT
public:
    EventWidget(QWidget *parent = nullptr);

    void setGlobalState(GlobalState *globalState);

public slots:
    void onEventsChanged();

protected slots:
    void onAddEventClicked();
    void onDelEventClicked();

signals:
    void eventAdded(const QString &key,
                    std::unique_ptr<PreconditionBase> *precondition,
                    std::unique_ptr<ActionBase> *action);

    void eventDeleted(const QString &key);

protected:
    void generateWidgets();
    void connectStuff();

    EventTableWidget *m_eventTable;

    QPushButton *m_addEvent;
    QPushButton *m_delEvent;

    QVBoxLayout *m_vLayout;

    GlobalState *m_globalState;

};

#endif // UNNAMED_PROJECT_EVENTWIDGET_H
