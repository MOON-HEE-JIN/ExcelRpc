#pragma once

#include <cstddef>
#include <string>

class MsvcRpcGenerator
{
public:
    bool GenerateAll();

    bool WriteStructures(const char* fileName = "StructDef.h");
    bool WriteProcedureEnums(const char* fileName = "PacketEnumDef.h");
    bool WriteEnums(const char* fileName = "EnumDef.h");
    bool WriteSerialization(const char* fileName = "Serialization");
    bool WriteClientToServerStub(const char* fileName = "SERVER_STUB");
    bool WriteServerToClientStub(const char* fileName = "CLIENT_STUB");

    void SetFileNamePrefix(std::string prefix);

private:
    std::string BuildFileName(const char* fileName) const;

    const std::string outputDirectory_ = "MSVC_FILE\\";
    std::string fileNamePrefix_;
};
