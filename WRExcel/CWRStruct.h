#pragma once
#include "CWRExcel.h"
#include "Def.h"


enum SHEET_STRUCT
{
	STRUCT,
	ENUM
};

class CWRRPCManager :
    public CWRExcel
{
public:
    CWRRPCManager() {};
    CWRRPCManager(const char* XlsxName);
    ~CWRRPCManager() {};
public:
    bool Init(std::string filename);
    void Readini(const char* iniFileName = ".\\CallerAndCallee.ini");

    int READ_GENERATOR();
    int READ_STRUCT();
    int READ_PROC_ENUM();
    int READ_ENUM();

    bool WRITE_STRUCT(const char* h_structdeffilename = "StructDef.h");
    bool WRITE_PROC_ENUM(const char* h_enumfileanme = "PacketEnumDef.h");
    bool WRITE_SERIALIZATION(std::string filename = "Serialization");

    bool CTSSTUB(std::string filename = "CTSRPC");  // Client to Server
    bool STCSTUB(std::string filename = "STCRPC");  // Server to Client
public:
    void ERROR_MESSAGE_CHECK(int error);

    xlsxioreadersheet sheet;
    
    const char* foldername = "NEW_FILE\\";
    
    std::map<std::string, std::string> m_map_Callee;

    std::map<std::string, std::list<Node>> m_map_Struct;
    std::map<std::string, std::list<ENode>> m_map_ProcEnum;
    std::map<std::string, std::list<ENode>> m_map_Enum;

    std::vector<std::string> m_vecStructName;

    std::map<std::string, std::string> m_map_STC_ETS;   // Server to Client , Enum to Struct
    std::string GetSTC_String(std::string str);
    std::map<std::string, std::string> m_map_CTS_ETS;   // Client to Server , Enum to Struct
    std::string GetCTS_String(std::string str);
    std::map<std::string, std::string> m_map_NULL_ETS;   // NULL
    std::map<std::string, std::string> m_map_ProjectDefineStruct;

    std::map<std::string, std::map<std::string, std::string>> m_mapTotalGenerator;
};
extern CWRRPCManager g_CWRRPCManager;
