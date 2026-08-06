#pragma once

#include <map>
#include <string>

class CSharpRpcGenerator
{
public:
    CSharpRpcGenerator();

    bool GenerateAll();
    bool WriteStructures(const char* fileName = "StructDef");
    bool WriteProcedureEnums(const char* fileName = "PacketEnumDef");
    bool WriteEnums(const char* fileName = "EnumDef");
    bool WriteSerialization(const char* fileName = "Serialization");
    bool WriteClientToServerStub(const char* fileName = "SERVER_STUB");
    bool WriteServerToClientStub(const char* fileName = "CLIENT_STUB");

private:
    const char* solutionName_ = "CSHAP_SERVER";
    const char* solutionFolderName_ = "Models";
    const char* outputDirectory_ = "CSHAP_FILE\\";

    std::map<std::string, std::string> littleEndianWriters_;
    std::map<std::string, std::string> littleEndianReaders_;
};
