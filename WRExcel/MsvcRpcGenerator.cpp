#include "MsvcRpcGenerator.h"

#include "RpcSchemaManager.h"

#include <cstdio>
#include <utility>

bool MsvcRpcGenerator::GenerateAll()
{
    bool success = true;
    success = WriteStructures() && success;
    success = WriteProcedureEnums() && success;
    success = WriteEnums() && success;
    success = WriteSerialization() && success;
    success = WriteClientToServerStub() && success;
    success = WriteServerToClientStub() && success;
    return success;
}

bool MsvcRpcGenerator::WriteStructures(const char* fileName)
{
	printf("=== WRITE MSVC STRUCT FILE ===\n");

	FILE* fp;
	const std::string outputPath = outputDirectory_ + BuildFileName(fileName);

	fopen_s(&fp, outputPath.c_str(), "w");
	if (fp == nullptr)
	{
		std::perror("Failed to open output file");
		return false;
	}

    fprintf(fp, "#pragma once\n\n");
    fprintf(fp, "#include <string>\n");
    fprintf(fp, "#include \"ProjectDefineStruct.h\"\n");
    for (const std::string& structureName : g_rpcSchemaManager.orderedStructureNames)
    {
        const auto iter = g_rpcSchemaManager.structures.find(structureName);
        if (iter == g_rpcSchemaManager.structures.end())
            continue;

        if (g_rpcSchemaManager.projectDefinedStructures.find(iter->first) != g_rpcSchemaManager.projectDefinedStructures.end())
            continue;

        fprintf(fp, "struct %s\n", iter->first.c_str());
        fprintf(fp, "{\n");

        std::list<FieldDefinition>::iterator list_iter = iter->second.begin();
        for (list_iter; list_iter != iter->second.end(); list_iter++)
        {
            if (list_iter->kind == FieldKind::StructureName)
                continue;

            std::string dataType = g_rpcSchemaManager.GetGeneratedType("MSVC", list_iter->typeName);

            if (list_iter->kind == FieldKind::Structure || list_iter->kind == FieldKind::StructureArray)
                dataType = list_iter->typeName;

            if (list_iter->kind == FieldKind::Array || list_iter->kind == FieldKind::StructureArray)
                fprintf(fp, "\t%s\t\t%s[%d];\n", dataType.c_str(), list_iter->variableName.c_str(), list_iter->arraySize);
            else
                fprintf(fp, "\t%s\t\t%s;\n", dataType.c_str(), list_iter->variableName.c_str());
        }

        fprintf(fp, "};\n");
    }

    StructureMap::iterator iter = g_rpcSchemaManager.structures.begin();
    for (iter; iter != g_rpcSchemaManager.structures.end(); iter++)
    {
        if (g_rpcSchemaManager.standaloneStructures.find(iter->first) != g_rpcSchemaManager.standaloneStructures.end())
            continue;
        if(g_rpcSchemaManager.projectDefinedStructures.find(iter->first) != g_rpcSchemaManager.projectDefinedStructures.end())
			continue;

        fprintf(fp, "struct %s\n", iter->first.c_str());
        fprintf(fp, "{\n");

        std::list<FieldDefinition>::iterator list_iter = iter->second.begin();
        for (list_iter; list_iter != iter->second.end(); list_iter++)
        {
            if (list_iter->kind == FieldKind::StructureName)
                continue;
            
            std::string dataType = g_rpcSchemaManager.GetGeneratedType("MSVC", list_iter->typeName);

            if (list_iter->kind == FieldKind::Structure || list_iter->kind == FieldKind::StructureArray)
                dataType = list_iter->typeName;

            if (list_iter->kind == FieldKind::Array || list_iter->kind == FieldKind::StructureArray)
            {

                fprintf(fp, "\t%s\t\t%s[%d];\n", dataType.c_str(), list_iter->variableName.c_str(), list_iter->arraySize);
            }
            else
            {

                fprintf(fp, "\t%s\t\t%s;\n", dataType.c_str(), list_iter->variableName.c_str());
            }
        }

        fprintf(fp, "};\n");
    }

    fclose(fp);
    return true;
}

bool MsvcRpcGenerator::WriteProcedureEnums(const char* fileName)
{
    printf("=== WRITE MSVC PROC ENUM FILE ===\n");

    FILE* fp;
    const std::string outputPath = outputDirectory_ + BuildFileName(fileName);

    fopen_s(&fp, outputPath.c_str(), "w");
    if (fp == nullptr)
    {
        std::perror("Failed to open output file");
        return false;
    }
    fprintf(fp, "#pragma once\n\n");

    EnumMap::iterator map_iter = g_rpcSchemaManager.procedureEnums.begin();
    for (map_iter; map_iter != g_rpcSchemaManager.procedureEnums.end(); map_iter++)
    {
        std::list<EnumEntry>::iterator list_iter;
        fprintf(fp, "namespace %s \n {\n", map_iter->first.c_str());
        std::string enumName = "\tenum " + map_iter->first + "\n\t{\n";
        fputs(enumName.c_str(), fp);

        for (list_iter = map_iter->second.begin();list_iter != map_iter->second.end(); list_iter++)
        {
            std::string enumContents;
            if (list_iter->useWideTabAlignment)
                enumContents = "\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t\t\t//" + list_iter->comment + "\n";
            else
                enumContents = "\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t//" + list_iter->comment + "\n";

            fputs(enumContents.c_str(), fp);
        }
        fprintf(fp, "\t};\n");
        fprintf(fp, "};\n\n");
    }

    fclose(fp);
    return true;
}

bool MsvcRpcGenerator::WriteEnums(const char* fileName)
{
    printf("=== WRITE MSVC ENUM FILE ===\n");

    FILE* fp;
    const std::string outputPath = outputDirectory_ + BuildFileName(fileName);

    fopen_s(&fp, outputPath.c_str(), "w");
    if (fp == nullptr)
    {
        std::perror("Failed to open output file");
        return false;
    }
    fprintf(fp, "#pragma once\n\n");

    EnumMap::iterator map_iter = g_rpcSchemaManager.enums.begin();
    for (map_iter; map_iter != g_rpcSchemaManager.enums.end(); map_iter++)
    {
        std::list<EnumEntry>::iterator list_iter;
        fprintf(fp, "namespace %s \n {\n", map_iter->first.c_str());
        std::string enumName = "\tenum " + map_iter->first + "\n\t{\n";
        fputs(enumName.c_str(), fp);

        for (list_iter = map_iter->second.begin(); list_iter != map_iter->second.end(); list_iter++)
        {
            std::string enumContents;
            if (list_iter->useWideTabAlignment)
                enumContents = "\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t\t\t//" + list_iter->comment + "\n";
            else
                enumContents = "\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t//" + list_iter->comment + "\n";

            fputs(enumContents.c_str(), fp);
        }
        fprintf(fp, "\t};\n");
        fprintf(fp, "};\n\n");
    }

    fclose(fp);
    return true;
}

bool MsvcRpcGenerator::WriteSerialization(const char* fileName)
{
    printf("=== WRITE MSVC SERIALIZATION FILE ===\n");

    const std::string generatedBaseName = BuildFileName(fileName);
    const std::string headerPath = outputDirectory_ + generatedBaseName + ".h";
    const std::string sourcePath = outputDirectory_ + generatedBaseName + ".cpp";

    // write header file
    {
        FILE* fp;
        fopen_s(&fp, headerPath.c_str(), "w");
        if (fp == nullptr)
        {
            std::perror("Failed to open output file");
            return false;
        }

        fprintf(fp, "#pragma once\n\n");

        const std::string structureHeaderFile = BuildFileName("StructDef.h");
        fprintf(fp, "#include \"%s\" \n\n", structureHeaderFile.c_str());

        //Serialization
        std::string functionPrefix = "int Serialization(char* buffer, ";

        StructureMap::iterator map_iter = g_rpcSchemaManager.structures.begin();
        for (map_iter; map_iter != g_rpcSchemaManager.structures.end(); map_iter++)
        {
            std::list<FieldDefinition>::iterator list_iter = map_iter->second.begin();

            std::string functionName;
            for (list_iter; list_iter != map_iter->second.end(); list_iter++)
            {
                if (list_iter->kind == FieldKind::StructureName)
                {
                    StructureMap::iterator iter = g_rpcSchemaManager.structures.find(list_iter->typeName);
                    if (g_rpcSchemaManager.projectDefinedStructures.find(iter->first) != g_rpcSchemaManager.projectDefinedStructures.end())
                    {
                        functionName = "static " + functionPrefix + list_iter->typeName + "& _value);";
                    }
                    else
                    {
                        functionName = functionPrefix + list_iter->typeName + "& _value);";
                    }

                }
            }
            functionName += "\n";
            fputs(functionName.c_str(), fp);
        }

        fprintf(fp, "\n\n\n");

        // UnSerialization
        {
            functionPrefix = "int UnSerialization(char* buffer, ";
            StructureMap::iterator unserializationIterator = g_rpcSchemaManager.structures.begin();
            for (;
                 unserializationIterator != g_rpcSchemaManager.structures.end();
                 ++unserializationIterator)
            {
                std::list<FieldDefinition>::iterator list_iter = unserializationIterator->second.begin();

                std::string functionName;
                for (; list_iter != unserializationIterator->second.end(); ++list_iter)
                {
                    if (list_iter->kind == FieldKind::StructureName)
                    {
                        functionName = functionPrefix + list_iter->typeName + "& _value);";
                    }
                }
                functionName += "\n";
                fputs(functionName.c_str(), fp);
            }
        }

        fclose(fp);
    }

    //  write cpp file
    {
        FILE* fp;
        fopen_s(&fp, sourcePath.c_str(), "w");
        if (fp == nullptr)
        {
            std::perror("Failed to open output file");
            return false;
        }

        fprintf(fp, "#include \"%s.h\" \n", generatedBaseName.c_str());
        
        const std::string enumHeaderFile = BuildFileName("PacketEnumDef.h");
        fprintf(fp, "#include \"%s\"\n", enumHeaderFile.c_str());
        fprintf(fp, "#include <memory.h>\n\n");

        const std::string functionPrefix = "int Serialization (char* buffer, ";
        StructureMap::iterator map_iter = g_rpcSchemaManager.structures.begin();
        for (map_iter; map_iter != g_rpcSchemaManager.structures.end(); map_iter++)
        {
            std::list<FieldDefinition>::iterator list_iter = map_iter->second.begin();

            std::string functionName;
            std::string functionBody;
            std::string enumName = "null";

            bool usesDynamicLength = false;
            std::string dynamicLengthFieldName = "0";
            for (list_iter; list_iter != map_iter->second.end(); list_iter++)
            {
                FieldKind kind = list_iter->kind;
                std::string name = list_iter->typeName;
                std::string variableName = list_iter->variableName;
                int arraySize = list_iter->arraySize;
                switch (kind)
                {
                case FieldKind::StructureName:
                {
                    functionName = functionPrefix + name + "& value)\n{\n";
                    if (name.find("CTS") != std::string::npos)
                        enumName = g_rpcSchemaManager.FindClientToServerEnum(name);
                    else
                        enumName = g_rpcSchemaManager.FindServerToClientEnum(name);

                    std::string initialSize = "0";
                    if (enumName != "null")
                    {
                        functionBody += "\tint hSize = 0;\n";
                        functionBody += "\tst_Header header;\n";   
                        initialSize = "sizeof(st_Header)";
                    }
                    functionBody += "\tint iSize = "+ initialSize +";\n";
                }
                    break;
                case FieldKind::Scalar:
                    if (list_iter->typeName == "net_Count")
                    {
                        usesDynamicLength = true;
                        dynamicLengthFieldName = variableName;
                    }
                    
                    if (list_iter->typeName == "net_string")
                    {
                        functionBody += "\tmemcpy(buffer + iSize, value." + variableName + ".c_str() , value." + variableName + ".length());\n";
                        functionBody += "\tiSize += value." + variableName + ".length();\n";
                    }
                    else
                    {
                        functionBody += "\tmemcpy(buffer + iSize, &value." + variableName + ", sizeof(value." + variableName + "));\n";
                        functionBody += "\tiSize += sizeof(value." + variableName + ");\n";
                    }
                    break;
                case FieldKind::Array:
                {
                    if (usesDynamicLength)
                    {
                        functionBody += "\tmemcpy(buffer + iSize, &value." + variableName + ","
                            + "sizeof(value." + variableName + "[0]) * value." + dynamicLengthFieldName + ");\n";
                        functionBody += "\tiSize += sizeof(value." + variableName + "[0]) *" + "value." + dynamicLengthFieldName + ";\n";
                    }
                    else
                    {
                        functionBody += "\tmemcpy(buffer + iSize, &value." + variableName + ","
                            + "sizeof(value." + variableName + "[0] * " + std::to_string(arraySize) + "));\n";
                        functionBody += "\tiSize += sizeof(value." + variableName + "[0]) *" + std::to_string(arraySize) + ";\n";
                    }
                }
                break;
                case FieldKind::Structure:
                    functionBody += "\tiSize += Serialization(buffer + iSize, value." + variableName + ");\n";
                    break;
                case FieldKind::StructureArray:
                    if (usesDynamicLength)
                    {
                        functionBody += "\tfor(int i = 0; i < value." + dynamicLengthFieldName + "; ++i)\n";
                        functionBody += "\t{\n";
                        functionBody += "\t\tiSize += Serialization(buffer + iSize, value." + variableName + "[i]);\n";
                        functionBody += "\t}\n";
                    }
                    else
                    {
                        functionBody += "\tfor(int i = 0; i < " + std::to_string(arraySize) + "; ++i)\n";
                        functionBody += "\t{\n";
                        functionBody += "\t\tiSize += Serialization(buffer + iSize, value." + variableName + "[i]);\n";
                        functionBody += "\t}\n";
                    }
                    break;
                default:
                    break;
                }
            }

            if (enumName != "null")
            {
                functionBody += "\n\theader.type = " + enumName + ";\n";
                functionBody += "\theader.size = iSize - sizeof(st_Header);\n";
                functionBody += "\tSerialization(buffer, header);\n";
            }

            fputs(functionName.c_str(), fp);
            fputs(functionBody.c_str(), fp);
            fprintf(fp, "\treturn iSize;\n");
            fprintf(fp, "}\n\n");
        }

        {

            //UnSerialization
            const std::string unserializationFunctionPrefix =
                "int UnSerialization (char* buffer, ";
            StructureMap::iterator unserializationIterator = g_rpcSchemaManager.structures.begin();
            for (;
                 unserializationIterator != g_rpcSchemaManager.structures.end();
                 ++unserializationIterator)
            {
                std::list<FieldDefinition>::iterator list_iter = unserializationIterator->second.begin();

                std::string functionName;
                std::string functionBody;

                bool usesDynamicLength = false;
                std::string dynamicLengthFieldName = "0";

                for (; list_iter != unserializationIterator->second.end(); ++list_iter)
                {
                    FieldKind kind = list_iter->kind;
                    std::string name = list_iter->typeName;
                    std::string variableName = list_iter->variableName;
                    int arraySize = list_iter->arraySize;
                    switch (kind)
                    {
                    case FieldKind::StructureName:
                        functionName = unserializationFunctionPrefix + name + "& value)\n{\n";
                        functionBody += "\tint iSize = 0;\n";
                        break;
                    case FieldKind::Scalar:
                        if (list_iter->typeName == "net_Count")
                        {
                            usesDynamicLength = true;
                            dynamicLengthFieldName = variableName;
                        }

                        if(list_iter->typeName == "net_string")
                        {
                            functionBody += "\tvalue."+ variableName +".assign(buffer + iSize, value.length);\n";
                            functionBody += "\tiSize += value.length;\n";
                        }
                        else
                        {
                            functionBody += "\tmemcpy(&value." + variableName + ", buffer + iSize" + ", sizeof(value." + variableName + "));\n";
                            functionBody += "\tiSize += sizeof(value." + variableName + ");\n";
                        }
                        break;
                    case FieldKind::Array:
                    {
                        if (usesDynamicLength)
                        {
                            functionBody += "\tmemcpy(&value." + variableName + ", buffer + iSize" + ","
                                + "sizeof(value." + variableName + "[0]) * value." + dynamicLengthFieldName + ");\n";
                            functionBody += "\tiSize += sizeof(value." + variableName + "[0]) * value." + dynamicLengthFieldName + ";\n";
                        }
                        else
                        {
                            functionBody += "\tmemcpy(&value." + variableName + ", buffer + iSize" + ","
                                + "sizeof(value." + variableName + "[0]) * " + std::to_string(arraySize) + ");\n";
                            functionBody += "\tiSize += sizeof(value." + variableName + "[0]) * " + std::to_string(arraySize) + ";\n";
                        }
                    }
                    break;
                    case FieldKind::Structure:
                        functionBody += "\tiSize += UnSerialization(buffer + iSize, value." + variableName + ");\n";
                        break;
                    case FieldKind::StructureArray:
                        if (usesDynamicLength)
                        {
                            functionBody += "\tfor(int i = 0; i < value." + dynamicLengthFieldName + "; ++i)\n";
                            functionBody += "\t{\n";
                            functionBody += "\t\tiSize += UnSerialization(buffer + iSize, value." + variableName + "[i]);\n";
                            functionBody += "\t}\n";
                        }
                        else
                        {
                            functionBody += "\tfor(int i = 0; i < " + std::to_string(arraySize) + "; ++i)\n";
                            functionBody += "{\n";
                            functionBody += "\t\tiSize += UnSerialization(buffer + iSize, value." + variableName + "[i]);\n";
                            functionBody += "}\n";
                        }
                        break;
                    default:
                        break;
                    }
                }

                fputs(functionName.c_str(), fp);
                fputs(functionBody.c_str(), fp);
                fprintf(fp, "\treturn iSize;\n");
                fprintf(fp, "}\n\n");
            }
        }
        fclose(fp);
    }
    return true;
}
bool MsvcRpcGenerator::WriteClientToServerStub(const char* fileName)
{
    printf("=== WRITE MSVC CTS_STUB FILE ===\n");
    // Client to Server
    // 
    //header file
    {
        FILE* fp;
        const std::string generatedBaseName = BuildFileName(fileName);
        const std::string outputPath = outputDirectory_ + generatedBaseName + ".h";

        fopen_s(&fp, outputPath.c_str(), "w");

        if (fp == nullptr)
        {
            std::perror("Failed to open output file");
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

        EnumMap::iterator enum_iter = g_rpcSchemaManager.procedureEnums.begin();
        for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
        {
            fprintf(fp, "\tvirtual void DO_%s_Proc(int type, Object* pTarget, Packet& cPacket);\n"
                , enum_iter->first.c_str());
        }

        // private
        fprintf(fp, "private:\n");
        enum_iter = g_rpcSchemaManager.procedureEnums.begin();
        for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
        {
            fprintf(fp, "\tstd::map<int, std::function<int(Object*, Packet&)>> m_map%sProc;\n"
                , enum_iter->first.c_str());
        }
        fprintf(fp, "\tvoid InitRegisterFuncPointer();\n");

        // private
        fprintf(fp, "private:\n");
        StringMap::iterator map_iter = g_rpcSchemaManager.clientToServerStructures.begin();
        for (map_iter; map_iter != g_rpcSchemaManager.clientToServerStructures.end(); map_iter++)
        {
            std::string functionName = map_iter->first;
            const std::size_t separator = functionName.find("::");
            if (separator != std::string::npos)
                functionName.replace(separator, 2, "_");
            
            fprintf(fp, "\tvirtual int DO_%s(Object* pTarget, Packet& pReqPacket) = 0;\n", functionName.c_str());
        }
        fprintf(fp, "\tvirtual int DO_ERROR_PACKET(Object* pTarget, Packet& pReqPacket) = 0;\n");
        fprintf(fp, "\tvirtual int DO_ERROR_RESULT(Object* pTarget, int ret, int type) = 0;\n");
        fprintf(fp, "\n};");
        fclose(fp);
    }

    //cpp file
    {
        const std::string generatedBaseName = BuildFileName(fileName);
        const std::string outputPath = outputDirectory_ + generatedBaseName + ".cpp";

        FILE* fp;
        fopen_s(&fp, outputPath.c_str(), "w");

        if (fp == nullptr)
        {
            std::perror("Failed to open output file");
            return false;
        }

        //header
        const std::string packetEnumFileName = BuildFileName("PacketEnumDef.h");
        fprintf(fp, "#include \"%s\"\n", packetEnumFileName.c_str());
        fprintf(fp, "#include \"%s.h\"\n\n", generatedBaseName.c_str());

        fprintf(fp, "template<typename Object, typename Packet>\n");
        fprintf(fp, "void Stub<Object,Packet>::InitRegisterFuncPointer()\n{\n");


        EnumMap::iterator procedureEnumIterator = g_rpcSchemaManager.procedureEnums.begin();
        for (procedureEnumIterator; procedureEnumIterator != g_rpcSchemaManager.procedureEnums.end(); procedureEnumIterator++)
        {
            if (g_rpcSchemaManager.callerTargets.find(procedureEnumIterator->first) == g_rpcSchemaManager.callerTargets.end())
                continue;

            std::list<EnumEntry>::iterator list_iter = procedureEnumIterator->second.begin();
            for (list_iter; list_iter != procedureEnumIterator->second.end(); list_iter++)
            {
                std::string structureName = procedureEnumIterator->first + "::" + list_iter->name.c_str();
                StringMap::iterator iter = g_rpcSchemaManager.clientToServerStructures.find(structureName);

                if (iter == g_rpcSchemaManager.clientToServerStructures.end())
                    continue;

                std::string functionName = structureName;
                const std::size_t separator = functionName.find("::");
                if (separator != std::string::npos)
                    functionName.replace(separator, 2, "_");

                fprintf(fp, "\tm_map%sProc[%s] = std::bind(&Stub::DO_%s, this, std::placeholders::_1, std::placeholders::_2);\n"
                    , procedureEnumIterator->first.c_str(), structureName.c_str(), functionName.c_str());
            }
        }
        fprintf(fp, "}\n");

        {
            EnumMap::iterator  enum_iter = g_rpcSchemaManager.procedureEnums.begin();
            for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
            {
                if (g_rpcSchemaManager.callerTargets.find(enum_iter->first) == g_rpcSchemaManager.callerTargets.end())
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

bool MsvcRpcGenerator::WriteServerToClientStub(const char* fileName)
{
    printf("=== WRITE MSVC STC_STUB FILE ===\n");

    // header
    {
        FILE* fp;
        const std::string generatedBaseName = BuildFileName(fileName);
        const std::string outputPath = outputDirectory_ + generatedBaseName + ".h";

        fopen_s(&fp, outputPath.c_str(), "w");

        if (fp == nullptr)
        {
            std::perror("Failed to open output file");
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

        EnumMap::iterator enum_iter = g_rpcSchemaManager.procedureEnums.begin();
        for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
        {
            fprintf(fp, "\tvirtual void DO_%s_Proc(int type, Object* pTarget, Packet& cPacket);\n"
                , enum_iter->first.c_str());
        }

        // private
        fprintf(fp, "private:\n");
        enum_iter = g_rpcSchemaManager.procedureEnums.begin();
        for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
        {
            fprintf(fp, "\tstd::map<int, std::function<int(Object*, Packet&)>> m_map%sProc;\n"
                , enum_iter->first.c_str());
        }
        fprintf(fp, "\tvoid InitRegisterFuncPointer();\n");

        // private
        fprintf(fp, "private:\n");
        StringMap::iterator map_iter = g_rpcSchemaManager.serverToClientStructures.begin();
        for (map_iter; map_iter != g_rpcSchemaManager.serverToClientStructures.end(); map_iter++)
        {
            std::string functionName = map_iter->first;
            const std::size_t separator = functionName.find("::");
            if (separator != std::string::npos)
                functionName.replace(separator, 2, "_");

            fprintf(fp, "\tvirtual int DO_%s(Object* pTarget, Packet& pReqPacket) = 0;\n"
                , functionName.c_str());
        }
        fprintf(fp, "\tvirtual int DO_ERROR_PACKET(Object* pTarget, Packet& pReqPacket) = 0;\n");
        fprintf(fp, "\tvirtual int DO_ERROR_RESULT(Object* pTarget, int ret, int type) = 0;\n");
        fprintf(fp, "\n};");
        fclose(fp);
    }

    //cpp file
    {
        const std::string generatedBaseName = BuildFileName(fileName);
        const std::string outputPath = outputDirectory_ + generatedBaseName + ".cpp";

        FILE* fp;
        fopen_s(&fp, outputPath.c_str(), "w");

        if (fp == nullptr)
        {
            std::perror("Failed to open output file");
            return false;
        }

        //header
        const std::string packetEnumFileName = BuildFileName("PacketEnumDef.h");
        fprintf(fp, "#include \"%s\"\n", packetEnumFileName.c_str());

        fprintf(fp, "#include \"%s.h\"\n", generatedBaseName.c_str());

        fprintf(fp, "template<typename Object, typename Packet>\n");
        fprintf(fp, "void Stub<Object,Packet>::InitRegisterFuncPointer()\n{\n");


        EnumMap::iterator procedureEnumIterator = g_rpcSchemaManager.procedureEnums.begin();
        for (procedureEnumIterator; procedureEnumIterator != g_rpcSchemaManager.procedureEnums.end(); procedureEnumIterator++)
        {
            std::list<EnumEntry>::iterator list_iter = procedureEnumIterator->second.begin();
            for (list_iter; list_iter != procedureEnumIterator->second.end(); list_iter++)
            {
                std::string structureName = procedureEnumIterator->first + "::" + list_iter->name.c_str();
                StringMap::iterator iter = g_rpcSchemaManager.serverToClientStructures.find(structureName);

                if (iter == g_rpcSchemaManager.serverToClientStructures.end())
                    continue;

                std::string functionName = structureName;
                const std::size_t separator = functionName.find("::");
                if (separator != std::string::npos)
                    functionName.replace(separator, 2, "_");

                fprintf(fp, "\tm_map%sProc[%s] = std::bind(&Stub::DO_%s, this, std::placeholders::_1, std::placeholders::_2);\n"
                    , procedureEnumIterator->first.c_str(), structureName.c_str(), functionName.c_str());
            }
        }
        fprintf(fp, "}\n");

        {
            EnumMap::iterator  enum_iter = g_rpcSchemaManager.procedureEnums.begin();
            for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
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

void MsvcRpcGenerator::SetFileNamePrefix(std::string prefix)
{
    fileNamePrefix_ = std::move(prefix);
}

std::string MsvcRpcGenerator::BuildFileName(const char* fileName) const
{
    return fileNamePrefix_ + fileName;
}
