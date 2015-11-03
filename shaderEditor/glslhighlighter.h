#ifndef GLSLHIGHLIGHTER_H
#define GLSLHIGHLIGHTER_H

#include <QObject>
#include "codeeditor.h"
#include <QString>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegExp>
#include <vector>

/**
 * based on: http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 * @brief The GLSLHighlighter class
 */

class GLSLHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    GLSLHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    std::vector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat preprocessorFormat;


signals:

public slots:
};

#endif // GLSLHIGHLIGHTER_H
