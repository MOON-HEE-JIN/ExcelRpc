#include "CMSVCRPC.h"

#include "CWRStruct.h"

bool CMSVCRPC::MSVC_WRITE_STRUCT(const char* h_structdeffilename)
{
	printf("=== WRITE MSVC STRUCT FILE ===\n");

	FILE* fp;
	char fp_name[512];
	sprintf(fp_name, "%s%s", foldername, h_structdeffilename);

	fopen_s(&fp, fp_name, "w");
	if (fp == nullptr)
	{
		g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
		return false;
	}

    fprintf(fp, "#pragma once\n\n");
    fprintf(fp, "#include <string>\n");
    int Loop = g_CWRRPCManager.m_vecStructName.size();
    for (int i = 0; i < Loop; i++)
    {
        STRUCTMAP_ITER iter = g_CWRRPCManager.m_map_Struct.find(g_CWRRPCManager.m_vecStructName[i]);

        fprintf(fp, "struct %s\n", iter->first.c_str());
        fprintf(fp, "{\n");

        STRUCTLIST_ITER list_iter = iter->second.begin();
        for (list_iter; list_iter != iter->second.end(); list_iter++)
        {
            if (list_iter->Type == Name)
                continue;

            std::string DataType = g_CWRRPCManager.m_mapTotalGenerator["MSVC"][list_iter->Name];

            if (list_iter->Type == Node_Type::Struct || list_iter->Type == Node_Type::StructArray)
                DataType = list_iter->Name;

            if (list_iter->Type == Node_Type::Array || list_iter->Type == Node_Type::StructArray)
                fprintf(fp, "\t%s\t\t%s[%d];\n", DataType.c_str(), list_iter->VariableName.c_str(), list_iter->ArraySize);
            else
                fprintf(fp, "\t%s\t\t%s;\n", DataType.c_str(), list_iter->VariableName.c_str());
        }

        fprintf(fp, "};\n");
    }

    STRUCTMAP_ITER iter = g_CWRRPCManager.m_map_Struct.begin();
    for (iter; iter != g_CWRRPCManager.m_map_Struct.end(); iter++)
    {
        if (g_CWRRPCManager.m_map_NULL_ETS.find(iter->first) != g_CWRRPCManager.m_map_NULL_ETS.end())
            continue;

        fprintf(fp, "struct %s\n", iter->first.c_str());
        fprintf(fp, "{\n");

        STRUCTLIST_ITER list_iter = iter->second.begin();
        for (list_iter; list_iter != iter->second.end(); list_iter++)
        {
            if (list_iter->Type == Name)
                continue;
            
            std::string DataType = g_CWRRPCManager.m_mapTotalGenerator["MSVC"][list_iter->Name];

            if (list_iter->Type == Node_Type::Struct || list_iter->Type == Node_Type::StructArray)
                DataType = list_iter->Name;

            if (list_iter->Type == Node_Type::Array || list_iter->Type == Node_Type::StructArray)
            {

                fprintf(fp, "\t%s\t\t%s[%d];\n", DataType.c_str(), list_iter->VariableName.c_str(), list_iter->ArraySize);
            }
            else
            {

                fprintf(fp, "\t%s\t\t%s;\n", DataType.c_str(), list_iter->VariableName.c_str());
            }
        }

        fprintf(fp, "};\n");
    }

    fclose(fp);
    return true;
}

bool CMSVCRPC::MSVC_WRITE_PROC_ENUM(const char* h_enumfileanme)
{
    printf("=== WRITE MSVC PROC ENUM FILE ===\n");

    FILE* fp;
    char fp_name[512];
    sprintf(fp_name, "%s%s", foldername, h_enumfileanme);

    fopen_s(&fp, fp_name, "w");
    if (fp == nullptr)
    {
        g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
        return false;
    }
    fprintf(fp, "#pragma once\n\n");

    ENUMMAP_ITER map_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
    for (map_iter; map_iter != g_CWRRPCManager.m_map_ProcEnum.end(); map_iter++)
    {
        ENUMLIST_ITER list_iter;
        fprintf(fp, "namespace %s \n {\n", map_iter->first.c_str());
        std::string enumName = "\tenum " + map_iter->first + "\n\t{\n";
        fprintf(fp, enumName.c_str());

        for (list_iter = map_iter->second.begin();list_iter != map_iter->second.end(); list_iter++)
        {
            std::string enumContents;
            if (list_iter->tab)
                enumContents = "\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t\t\t//" + list_iter->EComment + "\n";
            else
                enumContents = "\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t//" + list_iter->EComment + "\n";

            fprintf(fp, enumContents.c_str());
        }
        fprintf(fp, "\t};\n");
        fprintf(fp, "};\n\n");
    }

    fclose(fp);
    return true;
}

bool CMSVCRPC::MSVC_WRITE_ENUM(const char* filename)
{
    printf("=== WRITE MSVC ENUM FILE ===\n");

    FILE* fp;
    char fp_name[512];
    sprintf(fp_name, "%s%s", foldername, filename);

    fopen_s(&fp, fp_name, "w");
    if (fp == nullptr)
    {
        g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
        return false;
    }
    fprintf(fp, "#pragma once\n\n");

    ENUMMAP_ITER map_iter = g_CWRRPCManager.m_map_Enum.begin();
    for (map_iter; map_iter != g_CWRRPCManager.m_map_Enum.end(); map_iter++)
    {
        ENUMLIST_ITER list_iter;
        fprintf(fp, "namespace %s \n {\n", map_iter->first.c_str());
        std::string enumName = "\tenum " + map_iter->first + "\n\t{\n";
        fprintf(fp, enumName.c_str());

        for (list_iter = map_iter->second.begin(); list_iter != map_iter->second.end(); list_iter++)
        {
            std::string enumContents;
            if (list_iter->tab)
                enumContents = "\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t\t\t//" + list_iter->EComment + "\n";
            else
                enumContents = "\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t//" + list_iter->EComment + "\n";

            fprintf(fp, enumContents.c_str());
        }
        fprintf(fp, "\t};\n");
        fprintf(fp, "};\n\n");
    }

    fclose(fp);
    return true;
}

bool CMSVCRPC::MSVC_WRITE_SERIALIZATION(const char* filename)
{
    printf("=== WRITE MSVC SERIALIZATION FILE ===\n");

    char h_filename[512];
    char cpp_filename[512];
    sprintf(h_filename, "%s%s.h", foldername, filename);
    sprintf(cpp_filename, "%s%s.cpp", foldername, filename);

    // write header file
    {
        FILE* fp;
        fopen_s(&fp, h_filename, "w");
        if (fp == nullptr)
        {
            g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
            return false;
        }

        fprintf(fp, "#pragma once\n\n");
        fprintf(fp, "#include \"StructDef.h\" \n\n");


        //Serialization
        std::string D_funcname = "int Serialization(char* buffer, ";

        STRUCTMAP_ITER map_iter = g_CWRRPCManager.m_map_Struct.begin();
        for (map_iter; map_iter != g_CWRRPCManager.m_map_Struct.end(); map_iter++)
        {
            STRUCTLIST_ITER list_iter = map_iter->second.begin();

            std::string funcname;
            for (list_iter; list_iter != map_iter->second.end(); list_iter++)
            {
                if (list_iter->Type == Node_Type::Name)
                {
                    funcname = D_funcname + list_iter->Name + "& _value);";
                }
            }
            funcname += "\n";
            fprintf(fp, funcname.c_str());
        }

        fprintf(fp, "\n\n\n");

        // UnSerialization
        {
            D_funcname = "int UnSerialization(char* buffer, ";
            STRUCTMAP_ITER map_iter = g_CWRRPCManager.m_map_Struct.begin();
            for (map_iter; map_iter != g_CWRRPCManager.m_map_Struct.end(); map_iter++)
            {
                STRUCTLIST_ITER list_iter = map_iter->second.begin();

                std::string funcname;
                for (list_iter; list_iter != map_iter->second.end(); list_iter++)
                {
                    if (list_iter->Type == Node_Type::Name)
                    {
                        funcname = D_funcname + list_iter->Name + "& _value);";
                    }
                }
                funcname += "\n";
                fprintf(fp, funcname.c_str());
            }
        }

        fclose(fp);
    }

    //  write cpp file
    {
        FILE* fp;
        fopen_s(&fp, cpp_filename, "w");
        if (fp == nullptr)
        {
            g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
            return false;
        }

        fprintf(fp, "#include \"%s.h\" \n", filename);
        fprintf(fp, "#include \"PacketEnumDef.h\"\n");
        fprintf(fp, "#include <memory.h>\n\n");

        const std::string D_funcname = "int Serialization (char* buffer, ";
        STRUCTMAP_ITER map_iter = g_CWRRPCManager.m_map_Struct.begin();
        std::list<Node> header = g_CWRRPCManager.m_map_Struct["st_Header"];
        for (map_iter; map_iter != g_CWRRPCManager.m_map_Struct.end(); map_iter++)
        {
            STRUCTLIST_ITER list_iter = map_iter->second.begin();

            std::string funcname;
            std::string funccontents;
            std::string enumname = "null";

            for (list_iter; list_iter != map_iter->second.end(); list_iter++)
            {
                Node_Type type = list_iter->Type;
                std::string name = list_iter->Name;
                std::string vatablename = list_iter->VariableName;
                int arraysize = list_iter->ArraySize;
                switch (type)
                {
                case Node_Type::Name:
                {
                    funcname = D_funcname + name + "& value)\n{\n";
                    int strfindret = name.find("CTS");
                    if (strfindret != std::string::npos)
                        enumname = g_CWRRPCManager.GetCTS_String(name);
                    else
                        enumname = g_CWRRPCManager.GetSTC_String(name);

                    std::string strsize = "0";
                    if (enumname != "null")
                    {
                        funccontents += "\tint hSize = 0;\n";
                        funccontents += "\tst_Header header;\n";   
                        strsize = "sizeof(st_Header)";
                    }
                    funccontents += "\tint iSize = "+ strsize +";\n";
                }
                    break;
                case Node_Type::Variable:
                    if (list_iter->Name == "net_string")
                    {
                        funccontents += "\tmemcpy(buffer + iSize, value." + vatablename + ".c_str() , value." + vatablename + ".length());\n";
                        funccontents += "\tiSize += value." + vatablename + ".length();\n";
                    }
                    else
                    {
                        funccontents += "\tmemcpy(buffer + iSize, &value." + vatablename + ", sizeof(value." + vatablename + "));\n";
                        funccontents += "\tiSize += sizeof(value." + vatablename + ");\n";
                    }
                    break;
                case Node_Type::Array:
                {
                    funccontents += "\tmemcpy(buffer + iSize, &value." + vatablename + ","
                        + "sizeof(value." + vatablename + "[0] * " + std::to_string(arraysize) + "));\n";
                    funccontents += "\tiSize += sizeof(" + std::to_string(arraysize) + ");\n";
                }
                break;
                case Node_Type::Struct:
                    funccontents += "\tiSize += Serialization(buffer + iSize, value." + vatablename + ");\n";
                    break;
                case Node_Type::StructArray:
                    funccontents += "\tfor(int i = 0; i < " + std::to_string(arraysize) + "; ++i)\n";
                    funccontents += "\t{\n";
                    funccontents += "\t\tiSize += Serialization(buffer + iSize, value." + vatablename + "[i]);\n";
                    funccontents += "\t}\n";
                    break;
                default:
                    break;
                }
            }

            if (enumname != "null")
            {
                funccontents += "\n\theader.type = " + enumname + ";\n";
                funccontents += "\theader.size = iSize - sizeof(st_Header);\n";
                funccontents += "\tSerialization(buffer, header);\n";
            }

            fprintf(fp, funcname.c_str());
            fprintf(fp, funccontents.c_str());
            fprintf(fp, "\treturn iSize;\n");
            fprintf(fp, "}\n\n");
        }

        {

            //UnSerialization
            const std::string D_funcname = "int UnSerialization (char* buffer, ";
            STRUCTMAP_ITER map_iter = g_CWRRPCManager.m_map_Struct.begin();
            for (map_iter; map_iter != g_CWRRPCManager.m_map_Struct.end(); map_iter++)
            {
                STRUCTLIST_ITER list_iter = map_iter->second.begin();

                std::string funcname;
                std::string funccontents;
                for (list_iter; list_iter != map_iter->second.end(); list_iter++)
                {
                    Node_Type type = list_iter->Type;
                    std::string name = list_iter->Name;
                    std::string vatablename = list_iter->VariableName;
                    int arraysize = list_iter->ArraySize;
                    switch (type)
                    {
                    case Node_Type::Name:
                        funcname = D_funcname + name + "& value)\n{\n";
                        funccontents += "\tint iSize = 0;\n";
                        break;
                    case Node_Type::Variable:
                        if(list_iter->Name == "net_string")
                        {
                            funccontents += "\tvalue."+ vatablename +".assign(buffer + iSize, value.length);\n";
                            funccontents += "\tiSize += value.length;\n";
                        }
                        else
                        {
                            funccontents += "\tmemcpy(&value." + vatablename + ", buffer + iSize" + ", sizeof(value." + vatablename + "));\n";
                            funccontents += "\tiSize += sizeof(value." + vatablename + ");\n";
                        }
                        break;
                    case Node_Type::Array:
                    {
                        funccontents += "\tmemcpy(&value." + vatablename + ", buffer + iSize" + ","
                            + "sizeof(value." + vatablename + "[0] * " + std::to_string(arraysize) + "));\n";
                        funccontents += "\tiSize += sizeof(" + std::to_string(arraysize) + ");\n";
                    }
                    break;
                    case Node_Type::Struct:
                        funccontents += "\tiSize += UnSerialization(buffer + iSize, value." + vatablename + ");\n";
                        break;
                    case Node_Type::StructArray:
                        funccontents += "\tfor(int i = 0; i < " + std::to_string(arraysize) + "; ++i)\n";
                        funccontents += "{\n";
                        funccontents += "\t\tiSize += UnSerialization(buffer + iSize, value." + vatablename + "[i]);\n";
                        funccontents += "}\n";
                        break;
                    default:
                        break;
                    }
                }

                fprintf(fp, funcname.c_str());
                fprintf(fp, funccontents.c_str());
                fprintf(fp, "\treturn iSize;\n");
                fprintf(fp, "}\n\n");
            }
        }
        fclose(fp);
    }
    return true;
}
#pragma region TT

int a = 100;

#pragma endregion

bool CMSVCRPC::MSVC_WRITE_CTSSTUB(const char* filename)
{
    printf("=== WRITE MSVC CTS_STUB FILE ===\n");
    // Client to Server
    // 
    //header file
    {
        FILE* fp;
        char fp_name[512];
        sprintf(fp_name, "%s%s.h", foldername, filename);

        fopen_s(&fp, fp_name, "w");

        if (fp == nullptr)
        {
            g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
            return false;
        }

        fprintf(fp, "#pragma once\n\n");
        fprintf(fp, "#include <map>\n");
        fprintf(fp, "#include <functional> \n");

        // class
        fprintf(fp, "template<typename Object, typename Packet>\n");
        fprintf(fp, "class Stub\n{\n");

        // public
        fprintf(fp, "public:\n");
        fprintf(fp, "\tStub(){InitRegisterFuncPointer();}\n");

        ENUMMAP_ITER enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
        for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
        {
            fprintf(fp, "\tvirtual void DO_%s_Proc(int type, Object* pTarget, Packet& cPacket);\n"
                , enum_iter->first.c_str());
        }

        // private
        fprintf(fp, "private:\n");
        enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
        for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
        {
            fprintf(fp, "\tstd::map<int, std::function<int(Object*, Packet&)>> m_map%sProc;\n"
                , enum_iter->first.c_str());
        }
        fprintf(fp, "\tvoid InitRegisterFuncPointer();\n");

        // private
        fprintf(fp, "private:\n");
        STRSTR_ITER map_iter = g_CWRRPCManager.m_map_CTS_ETS.begin();
        for (map_iter; map_iter != g_CWRRPCManager.m_map_CTS_ETS.end(); map_iter++)
        {
            std::string funcname = map_iter->first;
            int c = funcname.find("::");
            funcname.replace(c, 2, "_");
            std::string Enum;
            char Name[256];
            funcname.copy(Name, c, 0);
            Name[c] = '\0';
            
            fprintf(fp, "\tvirtual int DO_%s(Object* pTarget, Packet& pReqPacket) = 0;\n", funcname.c_str());
        }
        fprintf(fp, "\tvirtual int DO_ERROR_PACKET(Object* pTarget, Packet& pReqPacket) = 0;\n");
        fprintf(fp, "\tvirtual int DO_ERROR_RESULT(Object* pTarget, int ret, int type) = 0;\n");
        fprintf(fp, "\n};");
        fclose(fp);
    }

    //cpp file
    {
        char fp_name[512];
        sprintf(fp_name, "%s%s.cpp", foldername, filename);
        FILE* fp;
        fopen_s(&fp, fp_name, "w");

        if (fp == nullptr)
        {
            g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
            return false;
        }

        //header
        fprintf(fp, "#include \"PacketEnumDef.h\"\n");
        fprintf(fp, "#include \"%s.h\"\n\n", filename);

        fprintf(fp, "template<typename Object, typename Packet>\n");
        fprintf(fp, "void Stub<Object,Packet>::InitRegisterFuncPointer()\n{\n");


        ENUMMAP_ITER ProcEnum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
        for (ProcEnum_iter; ProcEnum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); ProcEnum_iter++)
        {
            if (g_CWRRPCManager.m_map_Callee.find(ProcEnum_iter->first) == g_CWRRPCManager.m_map_Callee.end())
                continue;

            ENUMLIST_ITER list_iter = ProcEnum_iter->second.begin();
            for (list_iter; list_iter != ProcEnum_iter->second.end(); list_iter++)
            {
                std::string structname = ProcEnum_iter->first + "::" + list_iter->EName.c_str();
                STRSTR_ITER iter = g_CWRRPCManager.m_map_CTS_ETS.find(structname);

                if (iter == g_CWRRPCManager.m_map_CTS_ETS.end())
                    continue;

                std::string funcname = structname;
                int c = funcname.find("::");
                funcname.replace(c, 2, "_");

                fprintf(fp, "\tm_map%sProc[%s] = std::bind(&Stub::DO_%s, this, std::placeholders::_1, std::placeholders::_2);\n"
                    , ProcEnum_iter->first.c_str(), structname.c_str(), funcname.c_str());
            }
        }
        fprintf(fp, "}\n");

        {
            ENUMMAP_ITER  enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
            for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
            {
                if (g_CWRRPCManager.m_map_Callee.find(enum_iter->first) == g_CWRRPCManager.m_map_Callee.end())
                    continue;

                fprintf(fp, "template<typename Object, typename Packet>\n");
                fprintf(fp, "void Stub<Object,Packet>::DO_%s_Proc(int type, Object* pTarget, Packet& cPacket)\n"
                    , enum_iter->first.c_str());
                fprintf(fp, "{\n");
                fprintf(fp, "\tint ret = 0;\n");

                fprintf(fp, "\tif(m_map%sProc.find(type) == m_map%sProc.end())\n\t{\n", enum_iter->first.c_str(), enum_iter->first.c_str());
                fprintf(fp, "\t\tret = DO_ERROR_PACKET(pTarget, cPacket);\n");
                fprintf(fp, "\t\treturn;\n\t}\n");
                fprintf(fp, "\tret = m_map%sProc[type](pTarget, cPacket);\n", enum_iter->first.c_str());
                fprintf(fp, "\tif(ret != 0)");
                fprintf(fp, "\t\tDO_ERROR_RESULT(pTarget, ret, type);\n");
                fprintf(fp, "}\n");
            }
        }
        fclose(fp);
    }
    return true;
}

bool CMSVCRPC::MSVC_WRITE_STCSTUB(const char* filename)
{
    printf("=== WRITE MSVC STC_STUB FILE ===\n");

    // header
    {
        FILE* fp;
        char fp_name[512];
        sprintf(fp_name, "%s%s.h", foldername, filename);

        fopen_s(&fp, fp_name, "w");

        if (fp == nullptr)
        {
            g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
            return false;
        }

        fprintf(fp, "#pragma once\n\n");
        fprintf(fp, "#include <map>\n");
        fprintf(fp, "#include <functional> \n");

        // class
        fprintf(fp, "template<typename Object, typename Packet>\n");
        fprintf(fp, "class Stub\n{\n");

        // public
        fprintf(fp, "public:\n");
        fprintf(fp, "\tStub(){InitRegisterFuncPointer();}\n");

        ENUMMAP_ITER enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
        for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
        {
            fprintf(fp, "\tvirtual void DO_%s_Proc(int type, Object* pTarget, Packet& cPacket);\n"
                , enum_iter->first.c_str());
        }

        // private
        fprintf(fp, "private:\n");
        enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
        for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
        {
            fprintf(fp, "\tstd::map<int, std::function<int(Object*, Packet&)>> m_map%sProc;\n"
                , enum_iter->first.c_str());
        }
        fprintf(fp, "\tvoid InitRegisterFuncPointer();\n");

        // private
        fprintf(fp, "private:\n");
        STRSTR_ITER map_iter = g_CWRRPCManager.m_map_STC_ETS.begin();
        for (map_iter; map_iter != g_CWRRPCManager.m_map_STC_ETS.end(); map_iter++)
        {
            std::string funcname = map_iter->first;
            int c = funcname.find("::");
            funcname.replace(c, 2, "_");
            std::string Enum;
            char Name[256];
            funcname.copy(Name, c, 0);
            Name[c] = '\0';
            ENUMMAP_ITER enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();

            fprintf(fp, "\tvirtual int DO_%s(Object* pTarget, Packet& pReqPacket) = 0;\n"
                , funcname.c_str());
        }
        fprintf(fp, "\tvirtual int DO_ERROR_PACKET(Object* pTarget, Packet& pReqPacket) = 0;\n");
        fprintf(fp, "\tvirtual int DO_ERROR_RESULT(Object* pTarget, int ret, int type) = 0;\n");
        fprintf(fp, "\n};");
        fclose(fp);
    }

    //cpp file
    {
        char fp_name[512];
        sprintf(fp_name, "%s%s.cpp", foldername, filename);
        FILE* fp;
        fopen_s(&fp, fp_name, "w");

        if (fp == nullptr)
        {
            g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
            return false;
        }

        //header
        fprintf(fp, "#include \"PacketEnumDef.h\"\n");
        fprintf(fp, "#include \"%s.h\"\n", filename);

        fprintf(fp, "template<typename Object, typename Packet>\n");
        fprintf(fp, "void Stub<Object,Packet>::InitRegisterFuncPointer()\n{\n");


        ENUMMAP_ITER ProcEnum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
        for (ProcEnum_iter; ProcEnum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); ProcEnum_iter++)
        {
            ENUMLIST_ITER list_iter = ProcEnum_iter->second.begin();
            for (list_iter; list_iter != ProcEnum_iter->second.end(); list_iter++)
            {
                std::string structname = ProcEnum_iter->first + "::" + list_iter->EName.c_str();
                STRSTR_ITER iter = g_CWRRPCManager.m_map_STC_ETS.find(structname);

                if (iter == g_CWRRPCManager.m_map_STC_ETS.end())
                    continue;

                std::string funcname = structname;
                int c = funcname.find("::");
                funcname.replace(c, 2, "_");

                fprintf(fp, "\tm_map%sProc[%s] = std::bind(&Stub::DO_%s, this, std::placeholders::_1, std::placeholders::_2);\n"
                    , ProcEnum_iter->first.c_str(), structname.c_str(), funcname.c_str());
            }
        }
        fprintf(fp, "}\n");

        {
            ENUMMAP_ITER  enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
            for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
            {
                fprintf(fp, "template<typename Object, typename Packet>\n");
                fprintf(fp, "void Stub<Object,Packet>::DO_%s_Proc(int type, Object* pTarget, Packet& cPacket)\n"
                    , enum_iter->first.c_str());
                fprintf(fp, "{\n");
                fprintf(fp, "\tint ret = 0;\n");

                fprintf(fp, "\tif(m_map%sProc.find(type) == m_map%sProc.end())\n\t{\n", enum_iter->first.c_str(), enum_iter->first.c_str());
                fprintf(fp, "\t\tret = DO_ERROR_PACKET(pTarget, cPacket);\n");
                fprintf(fp, "\t\treturn;\n\t}\n");
                fprintf(fp, "\tret = m_map%sProc[type](pTarget, cPacket);\n", enum_iter->first.c_str());
                fprintf(fp, "\tif(ret != 0)");
                fprintf(fp, "\t\tDO_ERROR_RESULT(pTarget, ret, type);\n");
                fprintf(fp, "}\n");
            }
        }
        fclose(fp);
    }
    return true;
}
