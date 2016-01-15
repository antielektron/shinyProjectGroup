#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H

#include <memory>
#include <QDomElement>

#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Expressions/ExpressionConstant.h"

template <typename T>
class BinaryPreconditionBase : public PreconditionBase
{
public:
    typedef Expression<T> ExpressionType;

    BinaryPreconditionBase();
    BinaryPreconditionBase(const QDomElement &domElement);
    BinaryPreconditionBase(std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB);

    virtual ~BinaryPreconditionBase() {}

    virtual QString string() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

    inline Expression<T> *getLhs() { return m_exprA.get(); }
    inline Expression<T> *getRhs() { return m_exprB.get(); }

    void setLhs(std::unique_ptr<Expression<T>> expr) { m_exprA = std::move(expr); }
    void setRhs(std::unique_ptr<Expression<T>> expr) { m_exprB = std::move(expr); }

protected:
    std::unique_ptr<Expression<T>> m_exprA;
    std::unique_ptr<Expression<T>> m_exprB;
};


//------------------------------------------------------------------------------
template <typename T>
BinaryPreconditionBase<T>::BinaryPreconditionBase() :
        m_exprA(new ExpressionConstant<T>()),
        m_exprB(new ExpressionConstant<T>())
{}

//------------------------------------------------------------------------------
template <typename T>
BinaryPreconditionBase<T>::BinaryPreconditionBase(const QDomElement &domElement)
{
    // TODO expressions from DOM
}

//------------------------------------------------------------------------------
template <typename T>
BinaryPreconditionBase<T>::BinaryPreconditionBase(std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
        m_exprA(std::move(exprA)),
        m_exprB(std::move(exprB))
{}

//------------------------------------------------------------------------------
template <typename T>
QString BinaryPreconditionBase<T>::string()
{
    return m_exprA->string() + " " + this->type() + " " + m_exprB->string();
}

//------------------------------------------------------------------------------
template <typename T>
void BinaryPreconditionBase<T>::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Precondition");
    writer.writeAttribute("type", this->type());

    // TODO
    writer.writeAttribute("lhs", m_exprA->string());
    writer.writeAttribute("rhs", m_exprB->string());

    writer.writeEndElement();
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
