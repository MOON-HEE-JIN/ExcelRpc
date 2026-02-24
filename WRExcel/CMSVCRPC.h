#pragma once
class CMSVCRPC
{

public: //MSVC
    bool CreateFileMSVC();
private://MSVC
    const char* foldername = "MSVC_FILE\\";
public:
    bool MSVC_ALL_FILE()
    {
        MSVC_WRITE_STRUCT();
        MSVC_WRITE_PROC_ENUM();
        MSVC_WRITE_ENUM();
        MSVC_WRITE_SERIALIZATION();
        MSVC_WRITE_CTSSTUB();
        MSVC_WRITE_STCSTUB();

        return 0;
    };

    bool MSVC_WRITE_STRUCT(const char* h_structdeffilename = "StructDef.h");
    bool MSVC_WRITE_PROC_ENUM(const char* h_enumfileanme = "PacketEnumDef.h");
    bool MSVC_WRITE_ENUM(const char* filename = "EnumDef.h");
    bool MSVC_WRITE_SERIALIZATION(const char* filename = "Serialization");

    bool MSVC_WRITE_CTSSTUB(const char* filename = "SERVER_STUB");
    bool MSVC_WRITE_STCSTUB(const char* filename = "CLIENT_STUB");

};