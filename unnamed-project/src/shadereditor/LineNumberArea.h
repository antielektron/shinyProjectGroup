#ifndef UNNAMED_PROJECT_SHADER_EDITOR_LINE_NUMBER_AREA_H
#define UNNAMED_PROJECT_SHADER_EDITOR_LINE_NUMBER_AREA_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(CodeEditor);

/**
 * @brief The LineNumberArea class
 * source: http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    LineNumberArea(CodeEditor* editor);
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    CodeEditor *editor;
};

#endif // UNNAMED_PROJECT_SHADER_EDITOR_LINE_NUMBER_AREA_H
