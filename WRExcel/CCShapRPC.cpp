#include "CCshapRPC.h"
#include "CWRStruct.h"

CCshapRPC::CCshapRPC()
{
	printf("NET.8 == unsafe Build Mode\n");
	m_mapLittleEndian["Int16"] = WLittleEndianInt16;
	m_mapLittleEndian["UInt16"] = WLittleEndianUInt16;
	
	m_mapLittleEndian["Int32"] = WLittleEndianInt32;
	m_mapLittleEndian["UInt32"] = WLittleEndianUInt32;
	
	m_mapLittleEndian["Int64"] = WLittleEndianInt64;
	m_mapLittleEndian["UInt64"] = WLittleEndianUInt64;
	
	m_mapLittleEndian["float"] = WLittleEndianInt32;
	m_mapLittleEndian["double"] = WLittleEndianDouble;

	m_mapRLittleEndian["Int16"] = RLittleEndianInt16;
	m_mapRLittleEndian["UInt16"] = RLittleEndianUInt16;

	m_mapRLittleEndian["Int32"] = RLittleEndianInt32;
	m_mapRLittleEndian["UInt32"] = RLittleEndianUInt32;

	m_mapRLittleEndian["Int64"] = RLittleEndianInt64;
	m_mapRLittleEndian["UInt64"] = RLittleEndianUInt64;

	m_mapRLittleEndian["float"] = RLittleEndianInt32;
	m_mapRLittleEndian["double"] = RLittleEndianDouble;
}

bool CCshapRPC::CSHAP_WRITE_STRUCT(const char* filename)
{
	printf("=== WRITE C# STRUCT FILE ===\n");

	FILE* fp;
	char fp_name[512];
	sprintf(fp_name, "%s%s.cs", foldername, filename);

	fopen_s(&fp, fp_name, "w");
	if (fp == nullptr)
	{
		g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
		return false;
	}

	fprintf(fp, "namespace %s.%s", "STRUCT_DEF", SolutionFolderName);
	fprintf(fp, "{\n");

	int Loop = g_CWRRPCManager.m_vecStructName.size();
	for (int i = 0; i < Loop; i++)
	{
		STRUCTMAP_ITER stIter = g_CWRRPCManager.m_map_Struct.find(g_CWRRPCManager.m_vecStructName[i]);

		fprintf(fp, "\public struct %s\n", stIter->first.c_str());
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\t public %s(){}\n", stIter->first.c_str());
		STRUCTLIST_ITER list_iter = stIter->second.begin();
		for (list_iter; list_iter != stIter->second.end(); list_iter++)
		{
			if (list_iter->Type == Name)
				continue;

			std::string DataType = g_CWRRPCManager.m_mapTotalGenerator["Cshap"][list_iter->Name];

			if (list_iter->Type == Node_Type::Struct || list_iter->Type == Node_Type::StructArray)
				DataType = list_iter->Name;

			if (list_iter->Type == Node_Type::Array || list_iter->Type == Node_Type::StructArray)
				fprintf(fp, "\t\tpublic %s\t\t[]%s = new %s[%d];\n", DataType.c_str(), list_iter->VariableName.c_str(), DataType.c_str(), list_iter->ArraySize);
			else
				fprintf(fp, "\t\tpublic %s\t\t%s;\n", DataType.c_str(), list_iter->VariableName.c_str());
		}

		fprintf(fp, "\t}\n");
	}

	STRUCTMAP_ITER iter2 = g_CWRRPCManager.m_map_Struct.begin();
	for (iter2; iter2 != g_CWRRPCManager.m_map_Struct.end(); iter2++)
	{
		if (g_CWRRPCManager.m_map_NULL_ETS.find(iter2->first) != g_CWRRPCManager.m_map_NULL_ETS.end())
			continue;

		fprintf(fp, "\tpublic struct %s\n", iter2->first.c_str());
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\tpublic %s(){}\n", iter2->first.c_str());
		STRUCTLIST_ITER list_iter = iter2->second.begin();
		for (list_iter; list_iter != iter2->second.end(); list_iter++)
		{
			if (list_iter->Type == Name)
				continue;

			std::string DataType = g_CWRRPCManager.m_mapTotalGenerator["Cshap"][list_iter->Name];

			if (list_iter->Type == Node_Type::Struct || list_iter->Type == Node_Type::StructArray)
				DataType = list_iter->Name;

			if (list_iter->Type == Node_Type::Array || list_iter->Type == Node_Type::StructArray)
				fprintf(fp, "\t\tpublic %s\t\t[]%s = new %s[%d];\n", DataType.c_str(), list_iter->VariableName.c_str(), DataType.c_str(), list_iter->ArraySize);
			else
				fprintf(fp, "\t\tpublic %s\t\t%s;\n", DataType.c_str(), list_iter->VariableName.c_str());
		}
		fprintf(fp, "\t}\n");
	}

	fprintf(fp, "}");
	fclose(fp);
	return true;
}

bool CCshapRPC::CSHAP_WRITE_PROC_ENUM(const char* filename)
{
	printf("=== WRITE CSHAP PROC ENUM FILE ===\n");

	FILE* fp;
	char fp_name[512];
	sprintf(fp_name, "%s%s.cs", foldername, filename);

	fopen_s(&fp, fp_name, "w");
	if (fp == nullptr)
	{
		g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
		return false;
	}
	fprintf(fp, "namespace %s.%s\n", SolutionName, SolutionFolderName);
	fprintf(fp, "{\n");

	ENUMMAP_ITER iter = g_CWRRPCManager.m_map_ProcEnum.begin();
	for (iter; iter != g_CWRRPCManager.m_map_ProcEnum.end(); iter++)
	{
		ENUMLIST_ITER list_iter;
		fprintf(fp, "\tpublic enum %s\n", iter->first.c_str());
		fprintf(fp, "\t{\n");
		for (list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
		{
			std::string enumContents;
			if (list_iter->tab)
				enumContents = "\t\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t\t\t//" + list_iter->EComment + "\n";
			else
				enumContents = "\t\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t//" + list_iter->EComment + "\n";

			fprintf(fp, enumContents.c_str());
		}
		fprintf(fp, "\t}\n");
	}

	fprintf(fp, "}\n");

	fclose(fp);
	return true;
}

bool CCshapRPC::CSHAP_WRITE_ENUM(const char* filename)
{
	printf("=== WRITE CSHAP ENUM FILE ===\n");

	FILE* fp;
	char fp_name[512];
	sprintf(fp_name, "%s%s.cs", foldername, filename);

	fopen_s(&fp, fp_name, "w");
	if (fp == nullptr)
	{
		g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
		return false;
	}
	fprintf(fp, "namespace %s.%s\n", SolutionName, SolutionFolderName);
	fprintf(fp, "{\n");

	ENUMMAP_ITER iter = g_CWRRPCManager.m_map_Enum.begin();
	for (iter; iter != g_CWRRPCManager.m_map_Enum.end(); iter++)
	{
		ENUMLIST_ITER list_iter;
		fprintf(fp, "\tpublic enum %s\n", iter->first.c_str());
		fprintf(fp, "\t{\n");
		for (list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
		{
			std::string enumContents;
			if (list_iter->tab)
				enumContents = "\t\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t\t\t//" + list_iter->EComment + "\n";
			else
				enumContents = "\t\t\t" + list_iter->EName + list_iter->EValue + "\t\t\t//" + list_iter->EComment + "\n";

			fprintf(fp, enumContents.c_str());
		}
		fprintf(fp, "\t}\n");
	}

	fprintf(fp, "}\n");

	fclose(fp);
	return true;
}

bool CCshapRPC::CSHAP_WRITE_SERIALIZATION(const char* filename)
{
	printf("=== WRITE CSHAP SERIALIZATION FILE ===\n");
	char fp_name[512];
	sprintf(fp_name, "%s%s.cs",foldername, filename);
	
	FILE* fp;
	fopen_s(&fp, fp_name, "w");
	if (fp == nullptr)
	{
		g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
		return false;
	}

	fprintf(fp, "using System.Buffers.Binary;\n");
	fprintf(fp, "using System.Text;\n");
	fprintf(fp, "using STRUCT_DEF.%s;\n", SolutionFolderName);
	//fprintf(fp, "using %s.%s;", SolutionName, SolutionFolderName);
	fprintf(fp, "\n");
	fprintf(fp, "namespace %s.%s\n", SolutionName, SolutionFolderName);
	fprintf(fp, "{\n");

	{
		fprintf(fp, "\tpublic static class Serialization\n");
		fprintf(fp, "\t{\n");

		std::string FuncName = "\t\tpublic static int WritePacket(Span<byte> dst, ";

		STRUCTMAP_ITER struct_iter = g_CWRRPCManager.m_map_Struct.begin();
		for (struct_iter; struct_iter != g_CWRRPCManager.m_map_Struct.end(); struct_iter++)
		{
			STRUCTLIST_ITER list_iter = struct_iter->second.begin();

			std::string func;
			std::string func_define = "\t\t\tint offset = 0;\n";
			for (list_iter; list_iter != struct_iter->second.end(); list_iter++)
			{
				if (list_iter->Type == Node_Type::Name)
				{
					func += FuncName + list_iter->Name + " _value)";
				}
				else
				{
					std::string DataType = g_CWRRPCManager.m_mapTotalGenerator["Cshap"][list_iter->Name];

					if (list_iter->Type == Node_Type::Struct || list_iter->Type == Node_Type::StructArray)
						DataType = list_iter->Name;

					std::string strLittleEndian;
					if (list_iter->Type == Node_Type::Array || list_iter->Type == Node_Type::StructArray)
					{
						std::string formoon = "\t\t\tforeach(var v in _value." + list_iter->VariableName + ")\n\t\t\t{\n";

						if (m_mapLittleEndian.find(DataType) != m_mapLittleEndian.end())
						{
							strLittleEndian = m_mapLittleEndian[DataType];

							if (DataType == "float")
							{
								func_define += formoon + "\t\t\t\t" + strLittleEndian + "(dst.Slice(offset, sizeof("
									+ DataType + ")), BitConverter.SingleToInt32Bits(v)); offset += sizeof(" + DataType + ");\n\t\t\t}\n";
							}
							else
							{
								func_define += formoon + "\t\t\t\t" + strLittleEndian + "(dst.Slice(offset, sizeof("
									+ DataType + ")), v); offset += sizeof(" + DataType + ");\n\t\t\t}\n";
							}
						}
						else
						{
							//_value.ID.CopyTo(dst.Slice(offset, sizeof(Byte)* _value.ID.Length)); offset += sizeof(Byte) * _value.ID.Length;
							if (DataType == "Byte")
							{
								strLittleEndian = "_value." + list_iter->VariableName + ".CopyTo(dst.Slice(offset, sizeof(Byte) * _value." + list_iter->VariableName + ".Length)); offset += sizeof(Byte) * _value." + list_iter->VariableName + ".Length;";
								func_define += "\t\t\t" + strLittleEndian + "\n";
							}
							else
							{
								strLittleEndian = "offset += WritePacket(dst.Slice(offset), v);\n";
								func_define += formoon + "\t\t\t\t" + strLittleEndian + "\t\t\t}\n";
							}
						}
					}
					else
					{
						if (list_iter->Name == "net_string")
						{
							func_define += "\t\t\tEncoding.UTF8.GetBytes(_value.msg, dst.Slice(offset, _value.length));offset += _value.length;\n";
						}
						else if (m_mapLittleEndian.find(DataType) != m_mapLittleEndian.end())
						{
							strLittleEndian = m_mapLittleEndian[DataType];
							if (DataType == "float")
							{
								func_define += "\t\t\t" + strLittleEndian + "(dst.Slice(offset, sizeof("
									+ DataType + ")), BitConverter.SingleToInt32Bits(_value." + list_iter->VariableName + ")); offset += sizeof(" + DataType + ");\n";
							}
							else
							{
								func_define += "\t\t\t" + strLittleEndian + "(dst.Slice(offset, sizeof("
									+ DataType + ")), _value." + list_iter->VariableName + "); offset += sizeof(" + DataType + ");\n";
							}
						}
						else
						{
							//_value.ID.CopyTo(dst.Slice(offset, sizeof(Byte)* _value.ID.Length)); offset += sizeof(Byte) * _value.ID.Length;
							if (DataType == "Byte")
							{
								strLittleEndian = "_value." + list_iter->VariableName + ".CopyTo(dst.Slice(offset, sizeof(Byte))); offset += sizeof(Byte);";
								func_define += "\t\t\t" + strLittleEndian + "\n";
							}
							else
							{
								strLittleEndian = "offset += WritePacket(dst.Slice(offset), _value." + list_iter->VariableName;
								func_define += "\t\t\t" + strLittleEndian + ");\n";
							}
						}
					}

				}
			}
			func += "\n";
			fprintf(fp, func.c_str());
			fprintf(fp, "\t\t{\n");
			fprintf(fp, func_define.c_str());
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

		std::string FuncName = "\t\tpublic static int ReadPacket(ReadOnlySpan<byte> src, ref ";

		STRUCTMAP_ITER struct_iter = g_CWRRPCManager.m_map_Struct.begin();
		for (struct_iter; struct_iter != g_CWRRPCManager.m_map_Struct.end(); struct_iter++)
		{
			STRUCTLIST_ITER list_iter = struct_iter->second.begin();

			std::string func;
			std::string func_define = "\t\t\tint offset = 0;\n";
			for (list_iter; list_iter != struct_iter->second.end(); list_iter++)
			{
				if (list_iter->Type == Node_Type::Name)
				{
					func += FuncName + list_iter->Name + " _value)";
				}
				else
				{
					std::string DataType = g_CWRRPCManager.m_mapTotalGenerator["Cshap"][list_iter->Name];

					if (list_iter->Type == Node_Type::Struct || list_iter->Type == Node_Type::StructArray)
						DataType = list_iter->Name;

					std::string strLittleEndian;
					if (list_iter->Type == Node_Type::Array || list_iter->Type == Node_Type::StructArray)
					{
						std::string formoon = "\t\t\tfor(int i = 0; i < " + std::to_string(list_iter->ArraySize);
						formoon += +"; i++)\n\t\t\t{\n";

						if (m_mapRLittleEndian.find(DataType) != m_mapRLittleEndian.end())
						{
							strLittleEndian = m_mapRLittleEndian[DataType];

							if (DataType == "float")
							{
								func_define += formoon + "\t\t\t\tint bits = " + strLittleEndian + "(src.Slice(offset, sizeof("
									+ DataType + "))); offset += sizeof(" + DataType + ");\n";
								func_define += "\t\t\t_value." + list_iter->VariableName + "[i] = BitConverter.Int32BitsToSingle(bits);\n";
								func_define += "\t\t\t}\n";
							}
							else
							{
								func_define += formoon + "\t\t\t\t_value." + list_iter->VariableName + "[i] = " + strLittleEndian + "(src.Slice(offset, sizeof("
									+ DataType + "))); offset += sizeof(" + DataType + ");\n\t\t\t}\n";
							}
						}
						else
						{
							//_value.ID.CopyTo(dst.Slice(offset, sizeof(Byte)* _value.ID.Length)); offset += sizeof(Byte) * _value.ID.Length;
							if (DataType == "Byte")
							{
								//dst.Slice(offset, sizeof(Byte) * _value.aTest.Length).CopyTo(_value.aTest);
								strLittleEndian = "src.Slice(offset, sizeof(Byte) * _value." + list_iter->VariableName + ".Length).CopyTo(_value."+ list_iter->VariableName +"); " + "offset += sizeof(Byte) * _value." + list_iter->VariableName + ".Length;";
								func_define += "\t\t\t" + strLittleEndian + "\n";
							}
							else
							{
								strLittleEndian = "offset += ReadPacket(src.Slice(offset), ref _value. " + list_iter->VariableName + "[i]);\n";
								func_define += formoon + "\t\t\t\t" + strLittleEndian + "\t\t\t}\n";
							}
						}
					}
					else
					{
						if (list_iter->Name == "net_string")
						{
							func_define += "\t\t\t_value.msg = Encoding.UTF8.GetString(src.Slice(offset, _value.length));offset += _value.length;\n";
						}
						else if (m_mapRLittleEndian.find(DataType) != m_mapRLittleEndian.end())
						{
							strLittleEndian = m_mapRLittleEndian[DataType];
							if (DataType == "float")
							{
								func_define += "\t\t\t{\n";
								func_define += "\t\t\tint bits = " + strLittleEndian + "(src.Slice(offset, sizeof("
									+ DataType + "))); offset += sizeof(" + DataType + ");\n";
								func_define += "\t\t\t_value." + list_iter->VariableName + " = " + "BitConverter.Int32BitsToSingle(bits);\n";
								func_define += "\t\t\t}\n";
							}
							else
							{
								func_define += "\t\t\t_value." + list_iter->VariableName + " = " + strLittleEndian + "(src.Slice(offset, sizeof("
									+ DataType + "))" +"); offset += sizeof(" + DataType + ");\n";
							}
						}
						else
						{
							if (DataType == "Byte")
							{
								strLittleEndian = "src.Slice(offset, sizeof(Byte)).CopyTo(_value." + list_iter->VariableName + "); " + "sizeof(Byte);";
								func_define += "\t\t\t" + strLittleEndian + "\n";
							}
							else
							{
								strLittleEndian = "offset += ReadPacket(src.Slice(offset), ref _value." + list_iter->VariableName;
								func_define += "\t\t\t" + strLittleEndian + ");\n";
							}
						}
					}
				}
			}
			func += "\n";
			fprintf(fp, func.c_str());
			fprintf(fp, "\t\t{\n");
			fprintf(fp, func_define.c_str());
			fprintf(fp, "\t\t\treturn offset;\n");
			fprintf(fp, "\t\t}\n");
		}

		fprintf(fp, "\t}\n");
	}
	fprintf(fp, "}\n");
	
	fclose(fp);
	return true;
}

bool CCshapRPC::CSHAP_WRITE_CTSSTUB(const char* filename)
{
	printf("=== WRITE CSHAP SERIALIZATION FILE ===\n");
	char fp_name[512];
	sprintf(fp_name, "%s%s.cs", foldername, filename);

	FILE* fp;
	fopen_s(&fp, fp_name, "w");
	if (fp == nullptr)
	{
		g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
		return false;
	}
	std::string CShapRegisterFuncName = "InitRegisterFuncDictionary";

	fprintf(fp, "using System.Buffers.Binary;\n");
	fprintf(fp, "using System.Text;\n");
	fprintf(fp, "using %s.Models;", SolutionName);
	//fprintf(fp, "using STRUCT_DEF.%s;\n", SolutionFolderName);
	fprintf(fp, "\n");
	fprintf(fp, "namespace %s.%s\n", SolutionName, SolutionFolderName);
	fprintf(fp, "{\n");

	fprintf(fp, "\tpublic abstract class Stub<Object>\n");
	fprintf(fp, "\t{\n");

	ENUMMAP_ITER enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
	for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
	{
		fprintf(fp, "\t\tpublic readonly Dictionary<int, Func<Object, byte[], int>> m_Dic%sProc = new();\n"
			, enum_iter->first.c_str());
	}

	fprintf(fp, "\t\tpublic void %s()\n", CShapRegisterFuncName.c_str());
	fprintf(fp, "\t\t{\n");


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

			std::string enumname = structname;
			int b = enumname.find("::");
			enumname.replace(c, 2, ".");
			fprintf(fp, "\t\t\tm_Dic%sProc[(int)%s] = DO_%s;\n"
				, ProcEnum_iter->first.c_str(), enumname.c_str(), funcname.c_str());
		}
	}
	fprintf(fp, "\t\t}\n");


	fprintf(fp, "\t\tpublic Stub(){%s();}\n", CShapRegisterFuncName.c_str());

	enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
	for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
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
		
		fprintf(fp, "\t\tpublic abstract int DO_%s(Object pTarget, byte[] pReqPacket);\n"
			, funcname.c_str());
	}
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_PACKET(Object pTarget, byte[] pReqPacket);\n");
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_RESULT(Object pTarget, int ret, int type);\n");



	fprintf(fp, "\t}\n");

	fprintf(fp, "}");
	return true;
}

bool CCshapRPC::CSHAP_WRITE_STCSTUB(const char* filename)
{
	printf("=== WRITE CSHAP SERIALIZATION FILE ===\n");
	char fp_name[512];
	sprintf(fp_name, "%s%s.cs", foldername, filename);

	FILE* fp;
	fopen_s(&fp, fp_name, "w");
	if (fp == nullptr)
	{
		g_CWRRPCManager.ERROR_MESSAGE_CHECK(GetLastError());
		return false;
	}
	std::string CShapRegisterFuncName = "InitRegisterFuncDictionary";

	fprintf(fp, "using System.Buffers.Binary;\n");
	fprintf(fp, "using System.Text;\n");
	fprintf(fp, "using %s.Models;", SolutionName);
	//fprintf(fp, "using STRUCT_DEF.%s;\n", SolutionFolderName);
	fprintf(fp, "\n");
	fprintf(fp, "namespace %s.%s\n", SolutionName, SolutionFolderName);
	fprintf(fp, "{\n");

	fprintf(fp, "\tpublic abstract class Stub<Object>\n");
	fprintf(fp, "\t{\n");

	ENUMMAP_ITER enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
	for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
	{
		fprintf(fp, "\t\tpublic readonly Dictionary<int, Func<Object, byte[], int>> m_Dic%sProc = new();\n"
			, enum_iter->first.c_str());
	}

	fprintf(fp, "\t\tpublic void %s()\n", CShapRegisterFuncName.c_str());
	fprintf(fp, "\t\t{\n");


	ENUMMAP_ITER ProcEnum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
	for (ProcEnum_iter; ProcEnum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); ProcEnum_iter++)
	{
		if (g_CWRRPCManager.m_map_Callee.find(ProcEnum_iter->first) == g_CWRRPCManager.m_map_Callee.end())
			continue;

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

			std::string enumname = structname;
			int b = enumname.find("::");
			enumname.replace(c, 2, ".");
			fprintf(fp, "\t\t\tm_Dic%sProc[(int)%s] = DO_%s;\n"
				, ProcEnum_iter->first.c_str(), enumname.c_str(), funcname.c_str());
		}
	}
	fprintf(fp, "\t\t}\n");


	fprintf(fp, "\t\tpublic Stub(){%s();}\n", CShapRegisterFuncName.c_str());

	enum_iter = g_CWRRPCManager.m_map_ProcEnum.begin();
	for (enum_iter; enum_iter != g_CWRRPCManager.m_map_ProcEnum.end(); enum_iter++)
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
		
		fprintf(fp, "\t\tpublic abstract int DO_%s(Object pTarget, byte[] pReqPacket);\n"
			, funcname.c_str());
	}
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_PACKET(Object pTarget, byte[] pReqPacket);\n");
	fprintf(fp, "\t\tpublic abstract int DO_ERROR_RESULT(Object pTarget, int ret, int type);\n");

	fprintf(fp, "\t}\n");

	fprintf(fp, "}");
	return true;
}
