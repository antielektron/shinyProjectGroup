#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QPaintEvent>

QT_FORWARD_DECLARE_CLASS(LineNumberArea);

/**
 * @brief The CodeEditor class
 * source: http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    LineNumberArea *lineNumberArea;

protected slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
};

#endif // CODEEDITOR_H
