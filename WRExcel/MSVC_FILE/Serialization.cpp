#include "Serialization.h" 
#include "PacketEnumDef.h"
#include <memory.h>

int Serialization (char* buffer, st_CTS_ChangePid& value)
{
	int hSize = 0;
	st_Header header;
	int iSize = sizeof(st_Header);
	memcpy(buffer + iSize, &value.pid, sizeof(value.pid));
	iSize += sizeof(value.pid);

	header.type = GAME::CHANGEPID;
	header.size = iSize - sizeof(st_Header);
	Serialization(buffer, header);
	return iSize;
}

int Serialization (char* buffer, st_CTS_LoopBack& value)
{
	int hSize = 0;
	st_Header header;
	int iSize = sizeof(st_Header);
	memcpy(buffer + iSize, &value.zone, sizeof(value.zone));
	iSize += sizeof(value.zone);
	memcpy(buffer + iSize, &value.data, sizeof(value.data));
	iSize += sizeof(value.data);

	header.type = GAME::LOOPBACK;
	header.size = iSize - sizeof(st_Header);
	Serialization(buffer, header);
	return iSize;
}

int Serialization (char* buffer, st_Header& value)
{
	int iSize = 0;
	memcpy(buffer + iSize, &value.type, sizeof(value.type));
	iSize += sizeof(value.type);
	memcpy(buffer + iSize, &value.size, sizeof(value.size));
	iSize += sizeof(value.size);
	return iSize;
}

int Serialization (char* buffer, st_Info1& value)
{
	int iSize = 0;
	memcpy(buffer + iSize, &value.Loop1, sizeof(value.Loop1));
	iSize += sizeof(value.Loop1);
	memcpy(buffer + iSize, &value.ID,sizeof(value.ID[0]) * value.Loop1);
	iSize += sizeof(value.ID[0]) *value.Loop1;
	return iSize;
}

int Serialization (char* buffer, st_Info2& value)
{
	int iSize = 0;
	memcpy(buffer + iSize, &value.Loop1, sizeof(value.Loop1));
	iSize += sizeof(value.Loop1);
	for(int i = 0; i < value.Loop1; ++i)
	{
		iSize += Serialization(buffer + iSize, value.vec[i]);
	}
	return iSize;
}

int Serialization (char* buffer, st_Info3& value)
{
	int iSize = 0;
	memcpy(buffer + iSize, &value.Loop1, sizeof(value.Loop1));
	iSize += sizeof(value.Loop1);
	for(int i = 0; i < value.Loop1; ++i)
	{
		iSize += Serialization(buffer + iSize, value.vec[i]);
	}
	memcpy(buffer + iSize, &value.Loop2, sizeof(value.Loop2));
	iSize += sizeof(value.Loop2);
	for(int i = 0; i < value.Loop2; ++i)
	{
		iSize += Serialization(buffer + iSize, value.ID[i]);
	}
	return iSize;
}

int Serialization (char* buffer, st_STC_ChangePid& value)
{
	int hSize = 0;
	st_Header header;
	int iSize = sizeof(st_Header);
	memcpy(buffer + iSize, &value.ret, sizeof(value.ret));
	iSize += sizeof(value.ret);

	header.type = GAME::CHANGEPID;
	header.size = iSize - sizeof(st_Header);
	Serialization(buffer, header);
	return iSize;
}

int Serialization (char* buffer, st_STC_LoopBack& value)
{
	int hSize = 0;
	st_Header header;
	int iSize = sizeof(st_Header);
	memcpy(buffer + iSize, &value.ret, sizeof(value.ret));
	iSize += sizeof(value.ret);
	memcpy(buffer + iSize, &value.zone, sizeof(value.zone));
	iSize += sizeof(value.zone);
	memcpy(buffer + iSize, &value.data, sizeof(value.data));
	iSize += sizeof(value.data);

	header.type = GAME::LOOPBACK;
	header.size = iSize - sizeof(st_Header);
	Serialization(buffer, header);
	return iSize;
}

int Serialization (char* buffer, st_Vector& value)
{
	int iSize = 0;
	memcpy(buffer + iSize, &value.X, sizeof(value.X));
	iSize += sizeof(value.X);
	memcpy(buffer + iSize, &value.Y, sizeof(value.Y));
	iSize += sizeof(value.Y);
	return iSize;
}

int Serialization (char* buffer, st_Vector3F& value)
{
	int iSize = 0;
	memcpy(buffer + iSize, &value.X, sizeof(value.X));
	iSize += sizeof(value.X);
	memcpy(buffer + iSize, &value.Y, sizeof(value.Y));
	iSize += sizeof(value.Y);
	memcpy(buffer + iSize, &value.Z, sizeof(value.Z));
	iSize += sizeof(value.Z);
	return iSize;
}

int UnSerialization (char* buffer, st_CTS_ChangePid& value)
{
	int iSize = 0;
	memcpy(&value.pid, buffer + iSize, sizeof(value.pid));
	iSize += sizeof(value.pid);
	return iSize;
}

int UnSerialization (char* buffer, st_CTS_LoopBack& value)
{
	int iSize = 0;
	memcpy(&value.zone, buffer + iSize, sizeof(value.zone));
	iSize += sizeof(value.zone);
	memcpy(&value.data, buffer + iSize, sizeof(value.data));
	iSize += sizeof(value.data);
	return iSize;
}

int UnSerialization (char* buffer, st_Header& value)
{
	int iSize = 0;
	memcpy(&value.type, buffer + iSize, sizeof(value.type));
	iSize += sizeof(value.type);
	memcpy(&value.size, buffer + iSize, sizeof(value.size));
	iSize += sizeof(value.size);
	return iSize;
}

int UnSerialization (char* buffer, st_Info1& value)
{
	int iSize = 0;
	memcpy(&value.Loop1, buffer + iSize, sizeof(value.Loop1));
	iSize += sizeof(value.Loop1);
	memcpy(&value.ID, buffer + iSize,sizeof(value.ID[0]) * value.Loop1);
	iSize += sizeof(value.ID[0]) * value.Loop1;
	return iSize;
}

int UnSerialization (char* buffer, st_Info2& value)
{
	int iSize = 0;
	memcpy(&value.Loop1, buffer + iSize, sizeof(value.Loop1));
	iSize += sizeof(value.Loop1);
	for(int i = 0; i < value.Loop1; ++i)
	{
		iSize += UnSerialization(buffer + iSize, value.vec[i]);
	}
	return iSize;
}

int UnSerialization (char* buffer, st_Info3& value)
{
	int iSize = 0;
	memcpy(&value.Loop1, buffer + iSize, sizeof(value.Loop1));
	iSize += sizeof(value.Loop1);
	for(int i = 0; i < value.Loop1; ++i)
	{
		iSize += UnSerialization(buffer + iSize, value.vec[i]);
	}
	memcpy(&value.Loop2, buffer + iSize, sizeof(value.Loop2));
	iSize += sizeof(value.Loop2);
	for(int i = 0; i < value.Loop2; ++i)
	{
		iSize += UnSerialization(buffer + iSize, value.ID[i]);
	}
	return iSize;
}

int UnSerialization (char* buffer, st_STC_ChangePid& value)
{
	int iSize = 0;
	memcpy(&value.ret, buffer + iSize, sizeof(value.ret));
	iSize += sizeof(value.ret);
	return iSize;
}

int UnSerialization (char* buffer, st_STC_LoopBack& value)
{
	int iSize = 0;
	memcpy(&value.ret, buffer + iSize, sizeof(value.ret));
	iSize += sizeof(value.ret);
	memcpy(&value.zone, buffer + iSize, sizeof(value.zone));
	iSize += sizeof(value.zone);
	memcpy(&value.data, buffer + iSize, sizeof(value.data));
	iSize += sizeof(value.data);
	return iSize;
}

int UnSerialization (char* buffer, st_Vector& value)
{
	int iSize = 0;
	memcpy(&value.X, buffer + iSize, sizeof(value.X));
	iSize += sizeof(value.X);
	memcpy(&value.Y, buffer + iSize, sizeof(value.Y));
	iSize += sizeof(value.Y);
	return iSize;
}

int UnSerialization (char* buffer, st_Vector3F& value)
{
	int iSize = 0;
	memcpy(&value.X, buffer + iSize, sizeof(value.X));
	iSize += sizeof(value.X);
	memcpy(&value.Y, buffer + iSize, sizeof(value.Y));
	iSize += sizeof(value.Y);
	memcpy(&value.Z, buffer + iSize, sizeof(value.Z));
	iSize += sizeof(value.Z);
	return iSize;
}

