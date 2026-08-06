#pragma once

#include "ExcelWorkbookReader.h"
#include "Def.h"

#include <string>
#include <unordered_map>
#include <vector>

class RpcSchemaManager : public ExcelWorkbookReader
{
public:
    bool LoadSchema(const std::string& workbookFileName);
    void LoadCallerMappings(const char* iniFileName = ".\\CallerAndCallee.ini");

    std::string FindServerToClientEnum(const std::string& structureName) const;
    std::string FindClientToServerEnum(const std::string& structureName) const;
    std::string GetGeneratedType(
        const std::string& generatorName,
        const std::string& networkType) const;

    StringMap callerTargets;
    StructureMap structures;
    EnumMap procedureEnums;
    EnumMap enums;
    std::vector<std::string> orderedStructureNames;

    StringMap serverToClientStructures;
    StringMap clientToServerStructures;
    StringMap standaloneStructures;
    StringMap projectDefinedStructures;
    std::map<std::string, StringMap> generatedTypeMappings;

private:
    bool ReadTypeMappings();
    bool ReadStructures();
    bool ReadEnumSheet(const char* sheetName, EnumMap& destination);
    void ClearSchema();

    std::unordered_map<std::string, std::string> serverToClientEnumsByStructure_;
    std::unordered_map<std::string, std::string> clientToServerEnumsByStructure_;
};

extern RpcSchemaManager g_rpcSchemaManager;
