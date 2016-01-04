#ifndef UNNAMED_PROJECT_GAME_LOGIC_DATATYPES_H
#define UNNAMED_PROJECT_GAME_LOGIC_DATATYPES_H

#include <map>
#include <QString>

enum class AttributeDatatype
{
    Bool,
    Int,
    Float,
    QVector3D
};

const std::map<AttributeDatatype, QString> typeToQString =
{
    {AttributeDatatype::Bool, "bool"},
    {AttributeDatatype::Int, "int"},
    {AttributeDatatype::Float, "float"},
    {AttributeDatatype::QVector3D, "vector"}
};

const std::map<QString, AttributeDatatype> qStringToType =
{
    {"bool", AttributeDatatype::Bool},
    {"int", AttributeDatatype::Int},
    {"float", AttributeDatatype::Float},
    {"vector", AttributeDatatype::QVector3D}
};

#endif //UNNAMED_PROJECT_GAME_LOGIC_DATATYPES_H
