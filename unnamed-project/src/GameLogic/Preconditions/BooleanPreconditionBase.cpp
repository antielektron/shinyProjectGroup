#include "GameLogic/Preconditions/BooleanPreconditionBase.h"

//------------------------------------------------------------------------------
BooleanPreconditionBase::BooleanPreconditionBase()
{}

//------------------------------------------------------------------------------
BooleanPreconditionBase::BooleanPreconditionBase(const QDomElement &domElement)
{
    // TODO iterate over children, use factory!
}

//------------------------------------------------------------------------------
void BooleanPreconditionBase::addCondition(std::unique_ptr<PreconditionBase> condition)
{
    m_conditions.push_back(std::move(condition));
}

//------------------------------------------------------------------------------
void BooleanPreconditionBase::removeCondition(size_t i)
{
    m_conditions.erase(m_conditions.begin()+i);
}

//------------------------------------------------------------------------------
QString BooleanPreconditionBase::string()
{
    QString result;
    QString opName = this->type();

    for (auto it = m_conditions.begin(); it != m_conditions.end(); it++)
    {
        if (it != m_conditions.begin())
            result += opName;
        result += "(" + (*it)->string() + ")";
    }

    return result;
}

//------------------------------------------------------------------------------
void BooleanPreconditionBase::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Precondtion");
    writer.writeAttribute("type", this->type());

    for (auto &condition : m_conditions)
    {
        condition->writeToXml(writer);
    }

    writer.writeEndElement();
}