#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>

enum class FieldKind
{
    StructureName,
    Scalar,
    Array,
    Structure,
    StructureArray,
};

struct EnumEntry
{
    std::string name;
    std::string valueExpression;
    std::string comment;
    bool useWideTabAlignment = false;
};

struct FieldDefinition
{
    FieldKind kind = FieldKind::StructureName;
    std::string typeName;
    std::string variableName;
    int arraySize = 0;

    FieldDefinition() = default;

    explicit FieldDefinition(FieldKind fieldKind)
        : kind(fieldKind)
    {
    }

    FieldDefinition(FieldKind fieldKind, std::string fieldTypeName)
        : kind(fieldKind), typeName(std::move(fieldTypeName))
    {
    }

    FieldDefinition(FieldKind fieldKind, std::string fieldTypeName, std::string fieldVariableName)
        : kind(fieldKind),
          typeName(std::move(fieldTypeName)),
          variableName(std::move(fieldVariableName))
    {
    }

    FieldDefinition(
        FieldKind fieldKind,
        std::string fieldTypeName,
        std::string fieldVariableName,
        int fieldArraySize)
        : kind(fieldKind),
          typeName(std::move(fieldTypeName)),
          variableName(std::move(fieldVariableName)),
          arraySize(fieldArraySize)
    {
    }
};

using StructureMap = std::map<std::string, std::list<FieldDefinition>>;
using EnumMap = std::map<std::string, std::list<EnumEntry>>;
using StringMap = std::map<std::string, std::string>;
