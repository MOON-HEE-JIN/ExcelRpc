#pragma once
#include <map>;
#include <string>
class CCshapRPC
{
public:
	CCshapRPC();
	bool CreateFileCshap();
private:
	char SolutionName[512] = "CSHAP_SERVER";
	char SolutionFolderName[512] = "Models";
	const char* foldername = "CSHAP_FILE\\";
	

	std::map<std::string, std::string> m_mapLittleEndian;
	std::string LittleEndianByte;
	std::string WLittleEndianInt16 = "BinaryPrimitives.WriteInt16LittleEndian";
	std::string WLittleEndianUInt16 = "BinaryPrimitives.WriteUInt16LittleEndian";
	std::string WLittleEndianInt32 = "BinaryPrimitives.WriteInt32LittleEndian";
	std::string WLittleEndianUInt32 = "BinaryPrimitives.WriteUInt32LittleEndian";
	std::string WLittleEndianInt64 = "BinaryPrimitives.WriteInt64LittleEndian";
	std::string WLittleEndianUInt64 = "BinaryPrimitives.WriteUInt64LittleEndian";
	std::string WLittleEndianDouble = "BinaryPrimitives.WriteDoubleLittleEndian";

	//std::string WLittleEndianSingleFloat = "BinaryPrimitives.WriteSingleLittleEndian";
	
	std::map<std::string, std::string> m_mapRLittleEndian;
	std::string RLittleEndianInt16 = "BinaryPrimitives.ReadInt16LittleEndian";
	std::string RLittleEndianUInt16 = "BinaryPrimitives.ReadUInt16LittleEndian";
	std::string RLittleEndianInt32 = "BinaryPrimitives.ReadInt32LittleEndian";
	std::string RLittleEndianUInt32 = "BinaryPrimitives.ReadUInt32LittleEndian";
	std::string RLittleEndianInt64 = "BinaryPrimitives.ReadInt64LittleEndian";
	std::string RLittleEndianUInt64 = "BinaryPrimitives.ReadUInt64LittleEndian";
	std::string RLittleEndianDouble = "BinaryPrimitives.ReadDoubleLittleEndian";

public:
	bool CSHAP_ALL_FILE()
	{
		CSHAP_WRITE_STRUCT();
		CSHAP_WRITE_PROC_ENUM();
		CSHAP_WRITE_ENUM();
		CSHAP_WRITE_SERIALIZATION();

		CSHAP_WRITE_CTSSTUB();
		CSHAP_WRITE_STCSTUB();

		return true;
	}

	bool CSHAP_WRITE_STRUCT(const char* filename = "StructDef");
	bool CSHAP_WRITE_PROC_ENUM(const char* filename = "PacketEnumDef");
	bool CSHAP_WRITE_ENUM(const char* filename = "EnumDef");
	bool CSHAP_WRITE_SERIALIZATION(const char* filename = "Serialization");

	bool CSHAP_WRITE_CTSSTUB(const char* filename = "SERVER_STUB");
	bool CSHAP_WRITE_STCSTUB(const char* filename = "CLIENT_STUB");
};