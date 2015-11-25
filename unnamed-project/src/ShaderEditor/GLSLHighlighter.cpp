#include "ShaderEditor/GLSLHighlighter.h"

GLSLHighlighter::GLSLHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns
    << "\\battribute\\b"
        << "\\bconst\\b"
        << "\\buniform\\b"
        << "\\bvarying\\b"
        << "\\blayout\\b"
        << "\\bcentroid\\b"
        << "\\bflat\\b"
        << "\\bsmooth\\b"
        << "\\bnoperspective\\b"
        << "\\bpatch\\b"
        << "\\bsample\\b"
        << "\\bbreak\\b"
        << "\\bcontinue\\b"
        << "\\bdo\\b"
        << "\\bwhile\\b"
        << "\\bfor\\b"
        << "\\bswitch\\b"
        << "\\bcase\\b"
        << "\\bdefault\\b"
        << "\\bif\\b"
        << "\\belse\\b"
        << "\\bsubroutine\\b"
        << "\\bin\\b"
        << "\\bout\\b"
        << "\\binout\\b"
        << "\\bfloat\\b"
        << "\\bdouble\\b"
        << "\\bint\\b"
        << "\\bvoid\\b"
        << "\\bbool\\b"
        << "\\btrue\\b"
        << "\\bfalse\\b"
        << "\\binvariant\\b"
        << "\\bdiscard\\b"
        << "\\breturn\\b"
        << "\\b(|d)mat[2-4]\\b"
        << "\\b(|d)mat[2-4]x[2-4]\\b"
        << "\\b(|i|b|d|u)vec[2-4]\\b"
        << "\\buint\\b"
        << "\\blowp\\b"
        << "\\bmediump\\b"
        << "\\bhighp\\b"
        << "\\bprecision\\b"
        << "\\b(|i|u)sampler[1-3]D\\b"
        << "\\b(|i|u)samplerCube\\b"
        << "\\bsampler([1-2]D|Cube|)Shadow\\b"
        << "\\b(|i|u)sampler[1-2]DArray\\b"
        << "\\bsampler[1-2]DArrayShadow\\b"
        << "\\b(|u|i)sampler2DRect\\b"
        << "\\bsampler2DRectShadow\\b"
        << "\\b(|u|i)samplerBuffer\\b"
        << "\\b(|u|i)sampler2DMS(|Array)\\b"
        << "\\b(|u|i)samplerCubeArray\\b"
        << "\\bsamplerCubeArrayShadow\\b"
        << "\\bstruct\\b";


    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.push_back(rule);
    }

    //makes it sense to have string highlighting in glsl ?!?
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.push_back(rule);

    //preprocessor highlighting:
    preprocessorFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = preprocessorFormat;
    highlightingRules.push_back(rule);

    //function highlighting?
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.push_back(rule);

    //comment highlighting:
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.push_back(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

//=============================================================================

void GLSLHighlighter::highlightBlock(const QString &text)
{
    for (auto &rule : this->highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    //0 means, this is no comment block,
    //multi line comment blocks are marked with 1
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        startIndex = commentStartExpression.indexIn(text);
    }
    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text,startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                        + commentEndExpression.matchedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

//=============================================================================



