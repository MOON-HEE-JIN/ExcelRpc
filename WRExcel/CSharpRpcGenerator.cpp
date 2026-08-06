#include "CSharpRpcGenerator.h"
#include "RpcSchemaManager.h"

#include <cstdio>

CSharpRpcGenerator::CSharpRpcGenerator()
    : littleEndianWriters_{
          {"Int16", "BinaryPrimitives.WriteInt16LittleEndian"},
          {"UInt16", "BinaryPrimitives.WriteUInt16LittleEndian"},
          {"Int32", "BinaryPrimitives.WriteInt32LittleEndian"},
          {"UInt32", "BinaryPrimitives.WriteUInt32LittleEndian"},
          {"Int64", "BinaryPrimitives.WriteInt64LittleEndian"},
          {"UInt64", "BinaryPrimitives.WriteUInt64LittleEndian"},
          {"float", "BinaryPrimitives.WriteInt32LittleEndian"},
          {"double", "BinaryPrimitives.WriteDoubleLittleEndian"}},
      littleEndianReaders_{
          {"Int16", "BinaryPrimitives.ReadInt16LittleEndian"},
          {"UInt16", "BinaryPrimitives.ReadUInt16LittleEndian"},
          {"Int32", "BinaryPrimitives.ReadInt32LittleEndian"},
          {"UInt32", "BinaryPrimitives.ReadUInt32LittleEndian"},
          {"Int64", "BinaryPrimitives.ReadInt64LittleEndian"},
          {"UInt64", "BinaryPrimitives.ReadUInt64LittleEndian"},
          {"float", "BinaryPrimitives.ReadInt32LittleEndian"},
          {"double", "BinaryPrimitives.ReadDoubleLittleEndian"}}
{
}

bool CSharpRpcGenerator::GenerateAll()
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

bool CSharpRpcGenerator::WriteStructures(const char* fileName)
{
	printf("=== WRITE C# STRUCT FILE ===\n");

	FILE* fp;
	const std::string outputPath = std::string(outputDirectory_) + fileName + ".cs";

	fopen_s(&fp, outputPath.c_str(), "w");
	if (fp == nullptr)
	{
		std::perror("Failed to open output file");
		return false;
	}

	fprintf(fp, "namespace %s.%s\n", "STRUCT_DEF", solutionFolderName_);
	fprintf(fp, "{\n");

	for (const std::string& structureName : g_rpcSchemaManager.orderedStructureNames)
	{
		const auto stIter = g_rpcSchemaManager.structures.find(structureName);
		if (stIter == g_rpcSchemaManager.structures.end())
			continue;

		fprintf(fp, "\tpublic struct %s\n", stIter->first.c_str());
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\t public %s(){}\n", stIter->first.c_str());
		std::list<FieldDefinition>::iterator list_iter = stIter->second.begin();
		for (list_iter; list_iter != stIter->second.end(); list_iter++)
		{
			if (list_iter->kind == FieldKind::StructureName)
				continue;

			std::string dataType = g_rpcSchemaManager.GetGeneratedType("Cshap", list_iter->typeName);

			if (list_iter->kind == FieldKind::Structure || list_iter->kind == FieldKind::StructureArray)
				dataType = list_iter->typeName;

			if (list_iter->kind == FieldKind::Array || list_iter->kind == FieldKind::StructureArray)
				fprintf(fp, "\t\tpublic %s\t\t[]%s = new %s[%d];\n", dataType.c_str(), list_iter->variableName.c_str(), dataType.c_str(), list_iter->arraySize);
			else
				fprintf(fp, "\t\tpublic %s\t\t%s;\n", dataType.c_str(), list_iter->variableName.c_str());
		}

		fprintf(fp, "\t}\n");
	}

	StructureMap::iterator structureIterator = g_rpcSchemaManager.structures.begin();
	for (structureIterator; structureIterator != g_rpcSchemaManager.structures.end(); structureIterator++)
	{
		if (g_rpcSchemaManager.standaloneStructures.find(structureIterator->first) != g_rpcSchemaManager.standaloneStructures.end())
			continue;
		if (g_rpcSchemaManager.projectDefinedStructures.find(structureIterator->first) != g_rpcSchemaManager.projectDefinedStructures.end())
			continue;

		fprintf(fp, "\tpublic struct %s\n", structureIterator->first.c_str());
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\tpublic %s(){}\n", structureIterator->first.c_str());
		std::list<FieldDefinition>::iterator list_iter = structureIterator->second.begin();
		for (list_iter; list_iter != structureIterator->second.end(); list_iter++)
		{
			if (list_iter->kind == FieldKind::StructureName)
				continue;

			std::string dataType = g_rpcSchemaManager.GetGeneratedType("Cshap", list_iter->typeName);

			if (list_iter->kind == FieldKind::Structure || list_iter->kind == FieldKind::StructureArray)
				dataType = list_iter->typeName;

			if (list_iter->kind == FieldKind::Array || list_iter->kind == FieldKind::StructureArray)
				fprintf(fp, "\t\tpublic %s\t\t[]%s = new %s[%d];\n", dataType.c_str(), list_iter->variableName.c_str(), dataType.c_str(), list_iter->arraySize);
			else
				fprintf(fp, "\t\tpublic %s\t\t%s;\n", dataType.c_str(), list_iter->variableName.c_str());
		}
		fprintf(fp, "\t}\n");
	}

	fprintf(fp, "}");
	fclose(fp);
	return true;
}

bool CSharpRpcGenerator::WriteProcedureEnums(const char* fileName)
{
	printf("=== WRITE CSHAP PROC ENUM FILE ===\n");

	FILE* fp;
	const std::string outputPath = std::string(outputDirectory_) + fileName + ".cs";

	fopen_s(&fp, outputPath.c_str(), "w");
	if (fp == nullptr)
	{
		std::perror("Failed to open output file");
		return false;
	}
	fprintf(fp, "namespace %s.%s\n", solutionName_, solutionFolderName_);
	fprintf(fp, "{\n");

	EnumMap::iterator iter = g_rpcSchemaManager.procedureEnums.begin();
	for (iter; iter != g_rpcSchemaManager.procedureEnums.end(); iter++)
	{
		std::list<EnumEntry>::iterator list_iter;
		fprintf(fp, "\tpublic enum %s\n", iter->first.c_str());
		fprintf(fp, "\t{\n");
		for (list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
		{
			std::string enumContents;
			if (list_iter->useWideTabAlignment)
				enumContents = "\t\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t\t\t//" + list_iter->comment + "\n";
			else
				enumContents = "\t\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t//" + list_iter->comment + "\n";

			fputs(enumContents.c_str(), fp);
		}
		fprintf(fp, "\t}\n");
	}

	fprintf(fp, "}\n");

	fclose(fp);
	return true;
}

bool CSharpRpcGenerator::WriteEnums(const char* fileName)
{
	printf("=== WRITE CSHAP ENUM FILE ===\n");

	FILE* fp;
	const std::string outputPath = std::string(outputDirectory_) + fileName + ".cs";

	fopen_s(&fp, outputPath.c_str(), "w");
	if (fp == nullptr)
	{
		std::perror("Failed to open output file");
		return false;
	}
	fprintf(fp, "namespace %s.%s\n", solutionName_, solutionFolderName_);
	fprintf(fp, "{\n");

	EnumMap::iterator iter = g_rpcSchemaManager.enums.begin();
	for (iter; iter != g_rpcSchemaManager.enums.end(); iter++)
	{
		std::list<EnumEntry>::iterator list_iter;
		fprintf(fp, "\tpublic enum %s\n", iter->first.c_str());
		fprintf(fp, "\t{\n");
		for (list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
		{
			std::string enumContents;
			if (list_iter->useWideTabAlignment)
				enumContents = "\t\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t\t\t//" + list_iter->comment + "\n";
			else
				enumContents = "\t\t\t" + list_iter->name + list_iter->valueExpression + "\t\t\t//" + list_iter->comment + "\n";

			fputs(enumContents.c_str(), fp);
		}
		fprintf(fp, "\t}\n");
	}

	fprintf(fp, "}\n");

	fclose(fp);
	return true;
}

bool CSharpRpcGenerator::WriteSerialization(const char* fileName)
{
	printf("=== WRITE CSHAP SERIALIZATION FILE ===\n");
	const std::string outputPath = std::string(outputDirectory_) + fileName + ".cs";

	FILE* fp;
	fopen_s(&fp, outputPath.c_str(), "w");
	if (fp == nullptr)
	{
		std::perror("Failed to open output file");
		return false;
	}

	fprintf(fp, "using System.Buffers.Binary;\n");
	fprintf(fp, "using System.Text;\n");
	fprintf(fp, "using STRUCT_DEF.%s;\n", solutionFolderName_);
	fprintf(fp, "\n");
	fprintf(fp, "namespace %s.%s\n", solutionName_, solutionFolderName_);
	fprintf(fp, "{\n");

	{
		fprintf(fp, "\tpublic static class Serialization\n");
		fprintf(fp, "\t{\n");

		std::string functionPrefix = "\t\tpublic static int WritePacket(Span<byte> dst, ";

		StructureMap::iterator struct_iter = g_rpcSchemaManager.structures.begin();
		for (struct_iter; struct_iter != g_rpcSchemaManager.structures.end(); struct_iter++)
		{
			std::list<FieldDefinition>::iterator list_iter = struct_iter->second.begin();

			std::string func;
			std::string functionBody = "\t\t\tint offset = 0;\n";
			for (list_iter; list_iter != struct_iter->second.end(); list_iter++)
			{
				if (list_iter->kind == FieldKind::StructureName)
				{
					func += functionPrefix + list_iter->typeName + " _value)";
				}
				else
				{
					std::string dataType = g_rpcSchemaManager.GetGeneratedType("Cshap", list_iter->typeName);

					if (list_iter->kind == FieldKind::Structure || list_iter->kind == FieldKind::StructureArray)
						dataType = list_iter->typeName;

					std::string serializationExpression;
					if (list_iter->kind == FieldKind::Array || list_iter->kind == FieldKind::StructureArray)
					{
						std::string loopHeader = "\t\t\tforeach(var v in _value." + list_iter->variableName + ")\n\t\t\t{\n";

						if (littleEndianWriters_.find(dataType) != littleEndianWriters_.end())
						{
							serializationExpression = littleEndianWriters_[dataType];

							if (dataType == "float")
							{
								functionBody += loopHeader + "\t\t\t\t" + serializationExpression + "(dst.Slice(offset, sizeof("
									+ dataType + ")), BitConverter.SingleToInt32Bits(v)); offset += sizeof(" + dataType + ");\n\t\t\t}\n";
							}
							else
							{
								functionBody += loopHeader + "\t\t\t\t" + serializationExpression + "(dst.Slice(offset, sizeof("
									+ dataType + ")), v); offset += sizeof(" + dataType + ");\n\t\t\t}\n";
							}
						}
						else
						{
							if (dataType == "Byte")
							{
								serializationExpression = "_value." + list_iter->variableName + ".CopyTo(dst.Slice(offset, sizeof(Byte) * _value." + list_iter->variableName + ".Length)); offset += sizeof(Byte) * _value." + list_iter->variableName + ".Length;";
								functionBody += "\t\t\t" + serializationExpression + "\n";
							}
							else
							{
								serializationExpression = "offset += WritePacket(dst.Slice(offset), v);\n";
								functionBody += loopHeader + "\t\t\t\t" + serializationExpression + "\t\t\t}\n";
							}
						}
					}
					else
					{
						if (list_iter->typeName == "net_string")
						{
							functionBody += "\t\t\tEncoding.UTF8.GetBytes(_value.msg, dst.Slice(offset, _value.length));offset += _value.length;\n";
						}
						else if (littleEndianWriters_.find(dataType) != littleEndianWriters_.end())
						{
							serializationExpression = littleEndianWriters_[dataType];
							if (dataType == "float")
							{
								functionBody += "\t\t\t" + serializationExpression + "(dst.Slice(offset, sizeof("
									+ dataType + ")), BitConverter.SingleToInt32Bits(_value." + list_iter->variableName + ")); offset += sizeof(" + dataType + ");\n";
							}
							else
							{
								functionBody += "\t\t\t" + serializationExpression + "(dst.Slice(offset, sizeof("
									+ dataType + ")), _value." + list_iter->variableName + "); offset += sizeof(" + dataType + ");\n";
							}
						}
						else
						{
							if (dataType == "Byte")
							{
								serializationExpression = "_value." + list_iter->variableName + ".CopyTo(dst.Slice(offset, sizeof(Byte))); offset += sizeof(Byte);";
								functionBody += "\t\t\t" + serializationExpression + "\n";
							}
							else
							{
								serializationExpression = "offset += WritePacket(dst.Slice(offset), _value." + list_iter->variableName;
								functionBody += "\t\t\t" + serializationExpression + ");\n";
							}
						}
					}

				}
			}
			func += "\n";
			fputs(func.c_str(), fp);
			fprintf(fp, "\t\t{\n");
			fputs(functionBody.c_str(), fp);
			list_iter = struct_iter->second.begin();
			for (list_iter; list_iter != struct_iter->second.end(); list_iter++)
			{

			}
			fprintf(fp, "\t\t\treturn offset;\n");
			fprintf(fp, "\t\t}\n");
		}

		fprintf(fp, "\t}\n");
	}

	{
		fprintf(fp, "\tpublic static class UnSerialization\n");
		fprintf(fp, "\t{\n");

		std::string functionPrefix = "\t\tpublic static int ReadPacket(ReadOnlySpan<byte> src, ref ";

		StructureMap::iterator struct_iter = g_rpcSchemaManager.structures.begin();
		for (struct_iter; struct_iter != g_rpcSchemaManager.structures.end(); struct_iter++)
		{
			std::list<FieldDefinition>::iterator list_iter = struct_iter->second.begin();

			std::string func;
			std::string functionBody = "\t\t\tint offset = 0;\n";
			for (list_iter; list_iter != struct_iter->second.end(); list_iter++)
			{
				if (list_iter->kind == FieldKind::StructureName)
				{
					func += functionPrefix + list_iter->typeName + " _value)";
				}
				else
				{
					std::string dataType = g_rpcSchemaManager.GetGeneratedType("Cshap", list_iter->typeName);

					if (list_iter->kind == FieldKind::Structure || list_iter->kind == FieldKind::StructureArray)
						dataType = list_iter->typeName;

					std::string serializationExpression;
					if (list_iter->kind == FieldKind::Array || list_iter->kind == FieldKind::StructureArray)
					{
						std::string loopHeader = "\t\t\tfor(int i = 0; i < " + std::to_string(list_iter->arraySize);
						loopHeader += +"; i++)\n\t\t\t{\n";

						if (littleEndianReaders_.find(dataType) != littleEndianReaders_.end())
						{
							serializationExpression = littleEndianReaders_[dataType];

							if (dataType == "float")
							{
								functionBody += loopHeader + "\t\t\t\tint bits = " + serializationExpression + "(src.Slice(offset, sizeof("
									+ dataType + "))); offset += sizeof(" + dataType + ");\n";
								functionBody += "\t\t\t_value." + list_iter->variableName + "[i] = BitConverter.Int32BitsToSingle(bits);\n";
								functionBody += "\t\t\t}\n";
							}
							else
							{
								functionBody += loopHeader + "\t\t\t\t_value." + list_iter->variableName + "[i] = " + serializationExpression + "(src.Slice(offset, sizeof("
									+ dataType + "))); offset += sizeof(" + dataType + ");\n\t\t\t}\n";
							}
						}
						else
						{
							if (dataType == "Byte")
							{
								serializationExpression = "src.Slice(offset, sizeof(Byte) * _value." + list_iter->variableName + ".Length).CopyTo(_value."+ list_iter->variableName +"); " + "offset += sizeof(Byte) * _value." + list_iter->variableName + ".Length;";
								functionBody += "\t\t\t" + serializationExpression + "\n";
							}
							else
							{
								serializationExpression = "offset += ReadPacket(src.Slice(offset), ref _value. " + list_iter->variableName + "[i]);\n";
								functionBody += loopHeader + "\t\t\t\t" + serializationExpression + "\t\t\t}\n";
							}
						}
					}
					else
					{
						if (list_iter->typeName == "net_string")
						{
							functionBody += "\t\t\t_value.msg = Encoding.UTF8.GetString(src.Slice(offset, _value.length));offset += _value.length;\n";
						}
						else if (littleEndianReaders_.find(dataType) != littleEndianReaders_.end())
						{
							serializationExpression = littleEndianReaders_[dataType];
							if (dataType == "float")
							{
								functionBody += "\t\t\t{\n";
								functionBody += "\t\t\tint bits = " + serializationExpression + "(src.Slice(offset, sizeof("
									+ dataType + "))); offset += sizeof(" + dataType + ");\n";
								functionBody += "\t\t\t_value." + list_iter->variableName + " = " + "BitConverter.Int32BitsToSingle(bits);\n";
								functionBody += "\t\t\t}\n";
							}
							else
							{
								functionBody += "\t\t\t_value." + list_iter->variableName + " = " + serializationExpression + "(src.Slice(offset, sizeof("
									+ dataType + "))" +"); offset += sizeof(" + dataType + ");\n";
							}
						}
						else
						{
							if (dataType == "Byte")
							{
								serializationExpression = "src.Slice(offset, sizeof(Byte)).CopyTo(_value." + list_iter->variableName + "); " + "sizeof(Byte);";
								functionBody += "\t\t\t" + serializationExpression + "\n";
							}
							else
							{
								serializationExpression = "offset += ReadPacket(src.Slice(offset), ref _value." + list_iter->variableName;
								functionBody += "\t\t\t" + serializationExpression + ");\n";
							}
						}
					}
				}
			}
			func += "\n";
			fputs(func.c_str(), fp);
			fprintf(fp, "\t\t{\n");
			fputs(functionBody.c_str(), fp);
			fprintf(fp, "\t\t\treturn offset;\n");
			fprintf(fp, "\t\t}\n");
		}

		fprintf(fp, "\t}\n");
	}
	fprintf(fp, "}\n");
	
	fclose(fp);
	return true;
}

bool CSharpRpcGenerator::WriteClientToServerStub(const char* fileName)
{
	printf("=== WRITE C# CLIENT-TO-SERVER STUB FILE ===\n");
	const std::string outputPath = std::string(outputDirectory_) + fileName + ".cs";

	FILE* fp;
	fopen_s(&fp, outputPath.c_str(), "w");
	if (fp == nullptr)
	{
		std::perror("Failed to open output file");
		return false;
	}
	std::string registerFunctionName = "InitRegisterFuncDictionary";

	fprintf(fp, "using System.Buffers.Binary;\n");
	fprintf(fp, "using System.Text;\n");
	fprintf(fp, "using %s.Models;", solutionName_);
	fprintf(fp, "\n");
	fprintf(fp, "namespace %s.%s\n", solutionName_, solutionFolderName_);
	fprintf(fp, "{\n");

	fprintf(fp, "\tpublic abstract class Stub<Object>\n");
	fprintf(fp, "\t{\n");

	EnumMap::iterator enum_iter = g_rpcSchemaManager.procedureEnums.begin();
	for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
	{
		fprintf(fp, "\t\tpublic readonly Dictionary<int, Func<Object, byte[], int>> m_Dic%sProc = new();\n"
			, enum_iter->first.c_str());
	}

	fprintf(fp, "\t\tpublic void %s()\n", registerFunctionName.c_str());
	fprintf(fp, "\t\t{\n");


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

			std::string enumName = structureName;
			if (separator != std::string::npos)
				enumName.replace(separator, 2, ".");
			fprintf(fp, "\t\t\tm_Dic%sProc[(int)%s] = DO_%s;\n"
				, procedureEnumIterator->first.c_str(), enumName.c_str(), functionName.c_str());
		}
	}
	fprintf(fp, "\t\t}\n");


	fprintf(fp, "\t\tpublic Stub(){%s();}\n", registerFunctionName.c_str());

	enum_iter = g_rpcSchemaManager.procedureEnums.begin();
	for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
	{
		fprintf(fp, "\t\tpublic virtual int DO_%s_Proc(int type, Object pTarget, byte[] cPacket)\n"
			, enum_iter->first.c_str());
		fprintf(fp, "\t\t{\n");
		
		fprintf(fp, "\t\t\tint ret = 0;\n");

		fprintf(fp, "\t\t\tif(!m_Dic%sProc.ContainsKey(type))\n\t\t\t{\n", enum_iter->first.c_str());
		fprintf(fp, "\t\t\t\tret = DO_ERROR_PACKET(pTarget, cPacket);\n");
		fprintf(fp, "\t\t\t\treturn ret;\n\t\t\t}\n");
		fprintf(fp, "\t\t\tret = m_Dic%sProc[type](pTarget, cPacket);\n", enum_iter->first.c_str());
		fprintf(fp, "\t\t\tif(ret != 0)\n");
		fprintf(fp, "\t\t\t\tDO_ERROR_RESULT(pTarget, ret, type);\n");
		fprintf(fp, "\t\t\treturn ret;\n");

		fprintf(fp, "\t\t}\n");
	}

	StringMap::iterator map_iter = g_rpcSchemaManager.clientToServerStructures.begin();
	for (map_iter; map_iter != g_rpcSchemaManager.clientToServerStructures.end(); map_iter++)
	{
		std::string functionName = map_iter->first;
		const std::size_t separator = functionName.find("::");
		if (separator != std::string::npos)
			functionName.replace(separator, 2, "_");
		
		fprintf(fp, "\t\tpublic abstract int DO_%s(Object pTarget, byte[] pReqPacket);\n"
			, functionName.c_str());
	}
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_PACKET(Object pTarget, byte[] pReqPacket);\n");
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_RESULT(Object pTarget, int ret, int type);\n");



	fprintf(fp, "\t}\n");

	fprintf(fp, "}");
	fclose(fp);
	return true;
}

bool CSharpRpcGenerator::WriteServerToClientStub(const char* fileName)
{
	printf("=== WRITE C# SERVER-TO-CLIENT STUB FILE ===\n");
	const std::string outputPath = std::string(outputDirectory_) + fileName + ".cs";

	FILE* fp;
	fopen_s(&fp, outputPath.c_str(), "w");
	if (fp == nullptr)
	{
		std::perror("Failed to open output file");
		return false;
	}
	std::string registerFunctionName = "InitRegisterFuncDictionary";

	fprintf(fp, "using System.Buffers.Binary;\n");
	fprintf(fp, "using System.Text;\n");
	fprintf(fp, "using %s.Models;", solutionName_);
	fprintf(fp, "\n");
	fprintf(fp, "namespace %s.%s\n", solutionName_, solutionFolderName_);
	fprintf(fp, "{\n");

	fprintf(fp, "\tpublic abstract class Stub<Object>\n");
	fprintf(fp, "\t{\n");

	EnumMap::iterator enum_iter = g_rpcSchemaManager.procedureEnums.begin();
	for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
	{
		fprintf(fp, "\t\tpublic readonly Dictionary<int, Func<Object, byte[], int>> m_Dic%sProc = new();\n"
			, enum_iter->first.c_str());
	}

	fprintf(fp, "\t\tpublic void %s()\n", registerFunctionName.c_str());
	fprintf(fp, "\t\t{\n");


	EnumMap::iterator procedureEnumIterator = g_rpcSchemaManager.procedureEnums.begin();
	for (procedureEnumIterator; procedureEnumIterator != g_rpcSchemaManager.procedureEnums.end(); procedureEnumIterator++)
	{
		if (g_rpcSchemaManager.callerTargets.find(procedureEnumIterator->first) == g_rpcSchemaManager.callerTargets.end())
			continue;

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

			std::string enumName = structureName;
			if (separator != std::string::npos)
				enumName.replace(separator, 2, ".");
			fprintf(fp, "\t\t\tm_Dic%sProc[(int)%s] = DO_%s;\n"
				, procedureEnumIterator->first.c_str(), enumName.c_str(), functionName.c_str());
		}
	}
	fprintf(fp, "\t\t}\n");


	fprintf(fp, "\t\tpublic Stub(){%s();}\n", registerFunctionName.c_str());

	enum_iter = g_rpcSchemaManager.procedureEnums.begin();
	for (enum_iter; enum_iter != g_rpcSchemaManager.procedureEnums.end(); enum_iter++)
	{
		fprintf(fp, "\t\tpublic virtual int DO_%s_Proc(int type, Object pTarget, byte[] cPacket)\n"
			, enum_iter->first.c_str());
		fprintf(fp, "\t\t{\n");

		fprintf(fp, "\t\t\tint ret = 0;\n");

		fprintf(fp, "\t\t\tif(!m_Dic%sProc.ContainsKey(type))\n\t\t\t{\n", enum_iter->first.c_str());
		fprintf(fp, "\t\t\t\tret = DO_ERROR_PACKET(pTarget, cPacket);\n");
		fprintf(fp, "\t\t\t\treturn ret;\n\t\t\t}\n");
		fprintf(fp, "\t\t\tret = m_Dic%sProc[type](pTarget, cPacket);\n", enum_iter->first.c_str());
		fprintf(fp, "\t\t\tif(ret != 0)\n");
		fprintf(fp, "\t\t\t\tDO_ERROR_RESULT(pTarget, ret, type);\n");
		fprintf(fp, "\t\t\treturn ret;\n");

		fprintf(fp, "\t\t}\n");
	}

	StringMap::iterator map_iter = g_rpcSchemaManager.serverToClientStructures.begin();
	for (map_iter; map_iter != g_rpcSchemaManager.serverToClientStructures.end(); map_iter++)
	{
		std::string functionName = map_iter->first;
		const std::size_t separator = functionName.find("::");
		if (separator != std::string::npos)
			functionName.replace(separator, 2, "_");
		
		fprintf(fp, "\t\tpublic abstract int DO_%s(Object pTarget, byte[] pReqPacket);\n"
			, functionName.c_str());
	}
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_PACKET(Object pTarget, byte[] pReqPacket);\n");
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_RESULT(Object pTarget, int ret, int type);\n");

	fprintf(fp, "\t}\n");

	fprintf(fp, "}");
	fclose(fp);
	return true;
}
