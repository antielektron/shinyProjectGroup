#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_UNARY_BASE_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_UNARY_BASE_WIDGET_H

#include <memory>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class SceneEditorGame;

class UnaryBaseWidget : public QWidget
{
    Q_OBJECT
public:
    UnaryBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~UnaryBaseWidget();

protected slots:
    virtual void onApplyClicked() = 0;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;

    QLabel *m_type;
    QLineEdit *m_key;
    QPushButton *m_apply;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_UNARY_BASE_WIDGET_H
