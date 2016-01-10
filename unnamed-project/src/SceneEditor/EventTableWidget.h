#ifndef UNNAMED_EVENT_EVENTTABLEWIDGET_H
#define UNNAMED_EVENT_EVENTTABLEWIDGET_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include "GameLogic/GlobalState.h"

class EventTableWidget : public QTableView
{
    Q_OBJECT
public:
    EventTableWidget(QWidget *parent = nullptr);

    void init();

    void addItem(const QString &eventKey,
                 PreconditionBase *precondition,
                 ActionBase *action);

public slots:
    void onEventsChanged(GlobalState *globalState);

protected:
    QString actionToQString(ActionBase *action);
    void rebuidList(GlobalState *globalState);

    QStandardItemModel *m_model;
};

#endif // UNNAMED_EVENT_EVENTTABLEWIDGET_H
