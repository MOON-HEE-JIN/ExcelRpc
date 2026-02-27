#include "CWRStruct.h"

CWRRPCManager g_CWRRPCManager("RPC_GAME_EXCEL.xlsx");

CWRRPCManager::CWRRPCManager(const char* XlsxName)
{
    OpenXLSX(XlsxName);

    READ_GENERATOR();
    READ_STRUCT();
    READ_PROC_ENUM();
    READ_ENUM();
    Readini();
}

int CWRRPCManager::READ_STRUCT()
{
    XLSXIOCHAR* value;
    xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioread, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    while (xlsxioread_sheet_next_row(sheet))
    {
        int index = 0;              // 열 을 읽어올때 위치
        int variable_index = 0;     // 타입 or 이름
        bool bComment = false;      // 주석 여부
        bool bNull = false;         // 구조체 가 없을때
        char* enumType = nullptr;   // 구조체 enum 값

        std::string struct_name;
        std::string struct_enum;
        std::string comment;
        Node node;

        bool isDuplication = false;

        std::map<std::string, std::string>* pinsertMap = nullptr;
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
        {
            switch (index)
            {
            case 0:
            {
                enumType = strstr(value, "STC");
                if (enumType != nullptr)
                    pinsertMap = &m_map_STC_ETS;

                enumType = strstr(value, "CTS");
                if (enumType != nullptr)
                    pinsertMap = &m_map_CTS_ETS;

                enumType = strstr(value, "PROJECT");
                if(enumType != nullptr)
					pinsertMap = &m_map_ProjectDefineStruct;

                enumType = strstr(value, "NULL");
                if (enumType != nullptr)
                    pinsertMap = &m_map_NULL_ETS;
            }
            break;
            case 1: // enum namespace
            {
                enumType = strstr(value, "NULL");
                if (enumType != nullptr)
                    break;
                struct_enum = value;
            }
            break;
            case 2: // enum
            {
                enumType = strstr(value, "NULL");
                if (enumType != nullptr)
                    break;
                
                struct_enum += "::";
                struct_enum += value;
            }
            break;
            case 3: // struct name
            {
                enumType = strstr(value, "NULL");
                if (enumType != nullptr)
                {
                    if (pinsertMap == &m_map_NULL_ETS)
                    {
                        (*pinsertMap)[value] = value;
                        m_vecStructName.push_back(value);
                    }
                    else
                        (*pinsertMap)[struct_enum] = value;
                }
                else
                {
                    struct_name = value;
                    Node node(Node_Type::Name, struct_name);

                    if (m_map_Struct.find(struct_name) != m_map_Struct.end())
                    {
                        isDuplication = true;
                        break;
                    }
                    m_map_Struct[struct_name].push_back(node);
                    
                    if (pinsertMap == &m_map_NULL_ETS)
                    {
                        (*pinsertMap)[value] = value;
                        m_vecStructName.push_back(value);
                    }
                    else if (pinsertMap == &m_map_ProjectDefineStruct)
                    {
                        (*pinsertMap)[value] = value;
                        m_vecStructName.push_back(value);
                    }
                    else
                        (*pinsertMap)[struct_enum] = value;
                }
            }
            break;
            default:
            {
                if (value[0] == '/')
                {
                    comment += value;
                    bComment = true;
                }
                else
                {
                    // 변수 타입 읽을 차례
                    if (variable_index == 0)
                    {
                        node.Name = value;

                        STRUCTMAP_ITER map_iter = m_map_Struct.find(value);
                        
                        node.Type = Node_Type::Variable;
                        if (map_iter != m_map_Struct.end())
                            node.Type = Node_Type::Struct;
                        
                        variable_index++;
                    }
                    // 변수 이름 읽을 차례
                    else
                    {
                        // 변수가 배열인지 확인
                        char* array_ch = strstr(value, "[");

                        node.ArraySize = 0;
                        if (array_ch != nullptr)
                        {
                            char* array_ch_2 = strstr(value, "]");

                            *array_ch = '\0';
                            *array_ch_2 = '\0';

                            if (node.Type == Node_Type::Struct)
                                node.Type = Node_Type::StructArray;
                            else
                                node.Type = Node_Type::Array;

                            node.ArraySize = std::stoi(array_ch + 1);
                        }

                        node.VariableName = value;
                        
                        if (m_map_Struct.find(struct_name) != m_map_Struct.end())
                        {
                            m_map_Struct[struct_name].push_back(node);
                        }

                        variable_index = 0;
                    }
                }
            }
                break;
            }

            xlsxioread_free(value);
            index++;
            if (isDuplication)
                break;
        }
    }

    xlsxioread_sheet_close(sheet);
    return 0;
}

int CWRRPCManager::READ_PROC_ENUM()
{
    XLSXIOCHAR* value;
    xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioread, "ProcEnum", XLSXIOREAD_SKIP_EMPTY_ROWS);
    while (xlsxioread_sheet_next_row(sheet))
    {
        int index = 0;
        std::string name;

        ENode node;
        int Cnt = 0;
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
        {
            switch (index)
            {
            case 0:
                name = value;
                break;
            case 1:
                node.EName = value;
                break;
            case 2:
            {
                char* c = strstr(value, "NULL");

                if (c != nullptr)
                {
                    node.EValue = ",";
                    node.tab = true;
                    break;
                }
                node.EValue += " = ";
                node.EValue += value;
                node.EValue += ",";
                node.tab = false;
            }
                break;
            case 3:
            {
                node.EComment += value;
            }
                break;
            default:
                break;
            }
            index++;
            xlsxioread_free(value);
        }

        m_map_ProcEnum[name].push_back(node);
    }


    xlsxioread_sheet_close(sheet);
    return 0;
}

int CWRRPCManager::READ_ENUM()
{
    XLSXIOCHAR* value;
    xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioread, "Enum", XLSXIOREAD_SKIP_EMPTY_ROWS);
    while (xlsxioread_sheet_next_row(sheet))
    {
        int index = 0;
        std::string name;

        ENode node;
        int Cnt = 0;
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
        {
            switch (index)
            {
            case 0:
                name = value;
                break;
            case 1:
                node.EName = value;
                break;
            case 2:
            {
                char* c = strstr(value, "NULL");

                if (c != nullptr)
                {
                    node.EValue = ",";
                    node.tab = true;
                    break;
                }
                node.EValue += " = ";
                node.EValue += value;
                node.EValue += ",";
                node.tab = false;
            }
            break;
            case 3:
            {
                node.EComment += value;
            }
            break;
            default:
                break;
            }
            index++;
            xlsxioread_free(value);
        }

        m_map_Enum[name].push_back(node);
    }


    xlsxioread_sheet_close(sheet);
    return 0;
}

bool CWRRPCManager::Init(std::string filename)
{
    OpenXLSX(filename.c_str());

    m_map_Struct.clear();
    m_map_ProcEnum.clear();
    m_mapTotalGenerator.clear();

    m_map_STC_ETS.clear();
    m_map_CTS_ETS.clear();  
    m_map_NULL_ETS.clear();
    
    m_vecStructName.clear();

    READ_GENERATOR();
    READ_STRUCT();
    READ_PROC_ENUM();

    Readini();
    return false;
}

void CWRRPCManager::Readini(const char* iniFileName)
{

    ENUMMAP_ITER iter = m_map_ProcEnum.begin();
    for (iter; iter != m_map_ProcEnum.end(); iter++)
    {
        char buff[256];
        GetPrivateProfileStringA("CALLER", iter->first.c_str(), iter->first.c_str(), buff, 256, iniFileName);
        std::string callTarget = buff;
        m_map_Callee[iter->first.c_str()] = callTarget;
    }
    
    int a = 100;
    a++;
}

int CWRRPCManager::READ_GENERATOR()
{
    XLSXIOCHAR* value;
    xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioread, "NetTypeMappingGenerator", XLSXIOREAD_SKIP_EMPTY_ROWS);

    bool bSBuild = false;
    bool bCBuild = false;
    
    m_mapTotalGenerator;

    std::string GeneratorType = "None";
    std::string Build;
    while (xlsxioread_sheet_next_row(sheet))
    {
        if (GeneratorType == "None")
        {
            Build = xlsxioread_sheet_next_cell(sheet);
            GeneratorType = Build;
            continue;
        }

        int index = 0;
        
        std::string sNetDataType;
        std::string sBuildDataType;

        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
        {
            if (strcmp(value, "END") == 0)
            {
                GeneratorType = "None";
                xlsxioread_free(value);
                break;
            }

            switch (index)
            {
            case 0:
                sNetDataType = value;
                break;
            case 1:
                sBuildDataType = value;
                break;
            default:
                break;
            }
            index++;
            xlsxioread_free(value);
        }

        m_mapTotalGenerator[GeneratorType][sNetDataType] = sBuildDataType;
    }

    xlsxioread_sheet_close(sheet);
    return 0;
}


void CWRRPCManager::ERROR_MESSAGE_CHECK(int error)
{
    LPWSTR messagebuffer = nullptr;
    int size = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        (LPWSTR)&messagebuffer, 0, NULL
    );

    if (size == 0)
        printf("ERROR CODE = %d\n", error);
    std::wcout << L"ERROR MESSAGE : " << messagebuffer << std::endl;
    LocalFree(messagebuffer);
    if (error == 0x000bd000)
        exit(1);
}

std::string CWRRPCManager::GetSTC_String(std::string str)
{
    STRSTR_ITER iter = m_map_STC_ETS.begin();
    STRSTR_ITER eiter = m_map_STC_ETS.end();
    for (iter; iter != eiter; iter++)
    {
        if (str == iter->second)
            return iter->first;
    }
    return "null";
}

std::string CWRRPCManager::GetCTS_String(std::string str)
{
    STRSTR_ITER iter = m_map_CTS_ETS.begin();
    STRSTR_ITER eiter = m_map_CTS_ETS.end();
    for (iter; iter != eiter; iter++)
    {
        if (str == iter->second)
            return iter->first;
    }
    return "null";
}
