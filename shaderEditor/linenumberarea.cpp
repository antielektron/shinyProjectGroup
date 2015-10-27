#include "linenumberarea.h"
#include "codeeditor.h"

//=============================================================================

LineNumberArea::LineNumberArea(CodeEditor *editor) : QWidget(editor)
{
    this->editor = editor;
}

//=============================================================================

QSize LineNumberArea::sizeHint() const
{
    return QSize(editor->lineNumberAreaWidth(),0);
}

//=============================================================================

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    editor->lineNumberPaintEvent(event);
}
