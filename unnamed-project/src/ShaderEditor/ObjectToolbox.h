#ifndef OBJECTTOOLBOX_H
#define OBJECTTOOLBOX_H

#include <QObject>
#include <QDockWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QAction>
#include <QColumnView>

#include <memory>

class ObjectToolbox : public QDockWidget
{
    Q_OBJECT
public:
    ObjectToolbox(QWidget *parent = nullptr);
    ~ObjectToolbox();

protected:
    void createActions();
    void createToolbar();
    void createColumnView();
    void bakeUI();

    std::unique_ptr<QToolBar> m_toolbar;
    std::unique_ptr<QColumnView> m_columnView;
    std::unique_ptr<QWidget> m_multiWidget;
    std::unique_ptr<QVBoxLayout> m_layout;

    std::unique_ptr<QAction> m_addObjectAction;
    std::unique_ptr<QAction> m_delObjectAction;

public slots:
    /**
     * @brief adds a new Object to the scene
     */
    void addObject();

    /**
     * @brief delete selected Object from the Scene
     */
    void delObject();

};

#endif // OBJECTTOOLBOX_H
