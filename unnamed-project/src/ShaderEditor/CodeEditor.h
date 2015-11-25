#ifndef UNNAMED_PROJECT_SHADER_EDITOR_CODE_EDITOR_H
#define UNNAMED_PROJECT_SHADER_EDITOR_CODE_EDITOR_H

#include <QPlainTextEdit>
#include <QPaintEvent>

QT_FORWARD_DECLARE_CLASS(LineNumberArea);

/**
 * @brief The CodeEditor class
 * source: http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 *
 * includes fixes for working with QTextEdit, provided by
 * http://stackoverflow.com/a/24596246
 */
class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);

    int getFirstVisibleBlock();

    void lineNumberPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    LineNumberArea *lineNumberArea;

    int prevCursorLine;
    int currentCursorLine;

protected slots:


    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea();
    void onTextChanged();
    void onCursorChanged();

};

#endif // UNNAMED_PROJECT_SHADER_EDITOR_CODE_EDITOR_H
