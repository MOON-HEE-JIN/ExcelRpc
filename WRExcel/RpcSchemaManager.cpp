#include "RpcSchemaManager.h"

#include <cstdio>
#include <cstring>
#include <utility>

#include <windows.h>

namespace
{
constexpr const char* kServerToClientMarker = "STC";
constexpr const char* kClientToServerMarker = "CTS";
constexpr const char* kProjectMarker = "PROJECT";
constexpr const char* kNullMarker = "NULL";

bool Contains(const std::string& value, const char* marker)
{
    return value.find(marker) != std::string::npos;
}
}

RpcSchemaManager g_rpcSchemaManager;

bool RpcSchemaManager::LoadSchema(const std::string& workbookFileName)
{
    if (!OpenWorkbook(workbookFileName.c_str()))
    {
        return false;
    }

    ClearSchema();

    if (!ReadTypeMappings() ||
        !ReadStructures() ||
        !ReadEnumSheet("ProcEnum", procedureEnums) ||
        !ReadEnumSheet("Enum", enums))
    {
        return false;
    }

    LoadCallerMappings();
    return true;
}

void RpcSchemaManager::ClearSchema()
{
    callerTargets.clear();
    structures.clear();
    procedureEnums.clear();
    enums.clear();
    orderedStructureNames.clear();
    serverToClientStructures.clear();
    clientToServerStructures.clear();
    standaloneStructures.clear();
    projectDefinedStructures.clear();
    generatedTypeMappings.clear();
    serverToClientEnumsByStructure_.clear();
    clientToServerEnumsByStructure_.clear();
}

bool RpcSchemaManager::ReadStructures()
{
    xlsxioreadersheet sheet =
        xlsxioread_sheet_open(workbook_, nullptr, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (sheet == nullptr)
    {
        std::fprintf(stderr, "Failed to open the structure sheet.\n");
        return false;
    }

    while (xlsxioread_sheet_next_row(sheet))
    {
        int columnIndex = 0;
        int fieldTokenIndex = 0;
        std::string structureName;
        std::string enumIdentifier;
        FieldDefinition field;
        bool duplicateStructure = false;
        StringMap* targetMapping = nullptr;

        XLSXIOCHAR* rawValue = nullptr;
        while ((rawValue = xlsxioread_sheet_next_cell(sheet)) != nullptr)
        {
            const std::string value = rawValue;
            xlsxioread_free(rawValue);

            switch (columnIndex)
            {
            case 0:
                if (Contains(value, kServerToClientMarker))
                {
                    targetMapping = &serverToClientStructures;
                }
                else if (Contains(value, kClientToServerMarker))
                {
                    targetMapping = &clientToServerStructures;
                }
                else if (Contains(value, kProjectMarker))
                {
                    targetMapping = &projectDefinedStructures;
                }
                else if (Contains(value, kNullMarker))
                {
                    targetMapping = &standaloneStructures;
                }
                break;

            case 1:
                if (!Contains(value, kNullMarker))
                {
                    enumIdentifier = value;
                }
                break;

            case 2:
                if (!Contains(value, kNullMarker))
                {
                    enumIdentifier += "::" + value;
                }
                break;

            case 3:
                if (Contains(value, kNullMarker) || targetMapping == nullptr)
                {
                    break;
                }

                structureName = value;
                duplicateStructure = structures.find(structureName) != structures.end();
                if (duplicateStructure)
                {
                    break;
                }

                structures[structureName].emplace_back(FieldKind::StructureName, structureName);
                if (targetMapping == &standaloneStructures ||
                    targetMapping == &projectDefinedStructures)
                {
                    (*targetMapping)[structureName] = structureName;
                    orderedStructureNames.push_back(structureName);
                }
                else if (!enumIdentifier.empty())
                {
                    (*targetMapping)[enumIdentifier] = structureName;
                    if (targetMapping == &serverToClientStructures)
                    {
                        serverToClientEnumsByStructure_[structureName] = enumIdentifier;
                    }
                    else
                    {
                        clientToServerEnumsByStructure_[structureName] = enumIdentifier;
                    }
                }
                break;

            default:
                if (duplicateStructure || value.empty() || value.front() == '/')
                {
                    break;
                }

                if (fieldTokenIndex == 0)
                {
                    field.typeName = value;
                    field.kind = structures.find(value) == structures.end()
                        ? FieldKind::Scalar
                        : FieldKind::Structure;
                    fieldTokenIndex = 1;
                    break;
                }

                field.arraySize = 0;
                field.variableName = value;
                if (const std::size_t openBracket = value.find('[');
                    openBracket != std::string::npos)
                {
                    const std::size_t closeBracket = value.find(']', openBracket + 1);
                    if (closeBracket == std::string::npos)
                    {
                        std::fprintf(stderr, "Invalid array field: %s\n", value.c_str());
                        xlsxioread_sheet_close(sheet);
                        return false;
                    }

                    field.variableName = value.substr(0, openBracket);
                    try
                    {
                        field.arraySize = std::stoi(
                            value.substr(openBracket + 1, closeBracket - openBracket - 1));
                    }
                    catch (const std::exception&)
                    {
                        std::fprintf(stderr, "Invalid array size: %s\n", value.c_str());
                        xlsxioread_sheet_close(sheet);
                        return false;
                    }

                    field.kind = field.kind == FieldKind::Structure
                        ? FieldKind::StructureArray
                        : FieldKind::Array;
                }

                if (!structureName.empty())
                {
                    structures[structureName].push_back(std::move(field));
                }
                field = FieldDefinition{};
                fieldTokenIndex = 0;
                break;
            }

            ++columnIndex;
        }
    }

    xlsxioread_sheet_close(sheet);
    return true;
}

bool RpcSchemaManager::ReadEnumSheet(const char* sheetName, EnumMap& destination)
{
    xlsxioreadersheet sheet =
        xlsxioread_sheet_open(workbook_, sheetName, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (sheet == nullptr)
    {
        std::fprintf(stderr, "Failed to open sheet: %s\n", sheetName);
        return false;
    }

    while (xlsxioread_sheet_next_row(sheet))
    {
        int columnIndex = 0;
        std::string enumName;
        EnumEntry entry;

        XLSXIOCHAR* rawValue = nullptr;
        while ((rawValue = xlsxioread_sheet_next_cell(sheet)) != nullptr)
        {
            const std::string value = rawValue;
            xlsxioread_free(rawValue);

            switch (columnIndex)
            {
            case 0:
                enumName = value;
                break;
            case 1:
                entry.name = value;
                break;
            case 2:
                if (Contains(value, kNullMarker))
                {
                    entry.valueExpression = ",";
                    entry.useWideTabAlignment = true;
                }
                else
                {
                    entry.valueExpression = " = " + value + ",";
                }
                break;
            case 3:
                entry.comment += value;
                break;
            default:
                break;
            }

            ++columnIndex;
        }

        if (!enumName.empty() && !entry.name.empty())
        {
            destination[enumName].push_back(std::move(entry));
        }
    }

    xlsxioread_sheet_close(sheet);
    return true;
}

void RpcSchemaManager::LoadCallerMappings(const char* iniFileName)
{
    callerTargets.clear();
    for (const auto& [enumName, entries] : procedureEnums)
    {
        char buffer[256]{};
        GetPrivateProfileStringA(
            "CALLER",
            enumName.c_str(),
            enumName.c_str(),
            buffer,
            static_cast<DWORD>(sizeof(buffer)),
            iniFileName);
        callerTargets[enumName] = buffer;
    }
}

bool RpcSchemaManager::ReadTypeMappings()
{
    xlsxioreadersheet sheet = xlsxioread_sheet_open(
        workbook_, "NetTypeMappingGenerator", XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (sheet == nullptr)
    {
        std::fprintf(stderr, "Failed to open the type-mapping sheet.\n");
        return false;
    }

    std::string generatorType;
    while (xlsxioread_sheet_next_row(sheet))
    {
        if (generatorType.empty())
        {
            XLSXIOCHAR* rawGeneratorType = xlsxioread_sheet_next_cell(sheet);
            if (rawGeneratorType != nullptr)
            {
                generatorType = rawGeneratorType;
                xlsxioread_free(rawGeneratorType);
            }
            continue;
        }

        int columnIndex = 0;
        std::string networkType;
        std::string generatedType;
        bool reachedEnd = false;

        XLSXIOCHAR* rawValue = nullptr;
        while ((rawValue = xlsxioread_sheet_next_cell(sheet)) != nullptr)
        {
            const std::string value = rawValue;
            xlsxioread_free(rawValue);

            if (value == "END")
            {
                generatorType.clear();
                reachedEnd = true;
                break;
            }

            if (columnIndex == 0)
            {
                networkType = value;
            }
            else if (columnIndex == 1)
            {
                generatedType = value;
            }
            ++columnIndex;
        }

        if (!reachedEnd && !networkType.empty() && !generatedType.empty())
        {
            generatedTypeMappings[generatorType][networkType] = generatedType;
        }
    }

    xlsxioread_sheet_close(sheet);
    return true;
}

std::string RpcSchemaManager::FindServerToClientEnum(const std::string& structureName) const
{
    const auto iterator = serverToClientEnumsByStructure_.find(structureName);
    return iterator == serverToClientEnumsByStructure_.end() ? "null" : iterator->second;
}

std::string RpcSchemaManager::FindClientToServerEnum(const std::string& structureName) const
{
    const auto iterator = clientToServerEnumsByStructure_.find(structureName);
    return iterator == clientToServerEnumsByStructure_.end() ? "null" : iterator->second;
}

std::string RpcSchemaManager::GetGeneratedType(
    const std::string& generatorName,
    const std::string& networkType) const
{
    const auto generator = generatedTypeMappings.find(generatorName);
    if (generator == generatedTypeMappings.end())
    {
        return networkType;
    }

    const auto type = generator->second.find(networkType);
    return type == generator->second.end() ? networkType : type->second;
}
