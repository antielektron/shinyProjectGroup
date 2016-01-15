#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_BINARY_BASE_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_BINARY_BASE_WIDGET_H

#include <memory>

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class SceneEditorGame;

class BinaryBaseWidget : public QWidget
{
    Q_OBJECT
public:
    BinaryBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~BinaryBaseWidget();

protected slots:
    virtual void onApplyClicked() = 0;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;

    QLineEdit *m_lhs;
    QLineEdit *m_rhs;
    QLabel *m_type;

    QPushButton *m_apply;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_BINARY_BASE_WIDGET_H
