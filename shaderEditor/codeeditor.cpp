#include "codeeditor.h"
#include "linenumberarea.h"
#include <iostream>
#include <QTextBlock>
#include <QPainter>
#include <QScrollBar>

//=============================================================================

CodeEditor::CodeEditor(QWidget *parent) : QTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);


    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}

//=============================================================================

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

//=============================================================================

void CodeEditor::updateLineNumberAreaWidth(int newBlockCount)
{
    setViewportMargins(lineNumberAreaWidth(), 0,0,0);
}

//=============================================================================

void CodeEditor::updateLineNumberArea()
{
    //Q_UNUSED(sliderPos)

    //I'm not sure if this is really necessary, but according to
    //http://stackoverflow.com/a/24596246
    //it is.

    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QRect rect = this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(),rect.height());
    updateLineNumberAreaWidth(0);

    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1)
    {
        lineNumberArea->scroll(0,dy);
    }

    int firstVisibleBlockId = getFirstVisibleBlock();
    if (firstVisibleBlockId == 0 || this->textCursor().block().blockNumber() == firstVisibleBlockId-1)
    {
        this->verticalScrollBar()->setSliderPosition(dy-document()->documentMargin());
    }
}

//=============================================================================

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    QRect cr = contentsRect();

    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//=============================================================================

int CodeEditor::getFirstVisibleBlock()
{

    //TODO: this seems to be a horrible solution...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

//=============================================================================

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::blue).lighter(190);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//=============================================================================

void CodeEditor::lineNumberPaintEvent(QPaintEvent *event)
{

    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),Qt::lightGray);
    int firstVisibleBlock = this->getFirstVisibleBlock();

    QTextBlock block = document()->findBlockByNumber(firstVisibleBlock);
    int blockNumber = block.blockNumber();
    int top = this->viewport()->geometry().top();
    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    //TODO: get an offset while scrolling

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                         Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }


}
