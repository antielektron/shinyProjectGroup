#ifndef UNNAMED_PROJECT_SHADER_EDITOR_GLSL_HIGHLIGHTER_H
#define UNNAMED_PROJECT_SHADER_EDITOR_GLSL_HIGHLIGHTER_H

#include <vector>

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegExp>

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

#endif // UNNAMED_PROJECT_SHADER_EDITOR_GLSL_HIGHLIGHTER_H
