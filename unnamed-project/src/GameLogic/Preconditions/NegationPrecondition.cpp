#include "GameLogic/Preconditions/NegationPrecondition.h"

NegationPrecondition::NegationPrecondition(std::unique_ptr<PreconditionBase> condition) :
        m_condition(std::move(condition))
{
}

//------------------------------------------------------------------------------
NegationPrecondition::~NegationPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool NegationPrecondition::evaluateCondition()
{
    return !m_condition->evaluateCondition();
}

//------------------------------------------------------------------------------
QString NegationPrecondition::string()
{
    return this->type() + "(" + m_condition->string() + ")";
}

//------------------------------------------------------------------------------
QString NegationPrecondition::type()
{
    return traits::precondition_name<NegationPrecondition>::value;
}

//------------------------------------------------------------------------------
void NegationPrecondition::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Precondition");
    writer.writeAttribute("type", this->type());

    m_condition->writeToXml(writer);

    writer.writeEndElement();
}
