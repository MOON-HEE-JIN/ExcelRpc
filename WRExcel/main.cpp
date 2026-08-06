#include "CSharpRpcGenerator.h"
#include "MsvcRpcGenerator.h"
#include "RpcSchemaManager.h"

#include <cstdio>
#include <string>

namespace
{
constexpr const char* kDefaultWorkbook = "RPC_GAME_EXCEL.xlsx";
constexpr const char* kMsvcTarget = "msvc";
constexpr const char* kCSharpTarget = "csharp";
}

int main(int argumentCount, char* arguments[])
{
    const std::string workbookFileName =
        argumentCount > 1 ? arguments[1] : kDefaultWorkbook;
    const std::string generatorTarget =
        argumentCount > 2 ? arguments[2] : kMsvcTarget;

    if (!g_rpcSchemaManager.LoadSchema(workbookFileName))
    {
        return 1;
    }

    if (generatorTarget == kMsvcTarget)
    {
        MsvcRpcGenerator generator;
        return generator.GenerateAll() ? 0 : 1;
    }

    if (generatorTarget == kCSharpTarget)
    {
        CSharpRpcGenerator generator;
        return generator.GenerateAll() ? 0 : 1;
    }

    std::fprintf(
        stderr,
        "Unknown generator target '%s'. Use 'msvc' or 'csharp'.\n",
        generatorTarget.c_str());
    return 1;
}
