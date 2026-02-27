#pragma once

#include <string>
#include "ProjectDefineStruct.h"
struct st_Header
{
	__int32		type;
	__int32		size;
};
struct st_Vector
{
	float		X;
	float		Y;
};
struct st_Info1
{
	__int32		Loop1;
	__int32		ID[50];
};
struct st_Info2
{
	__int32		Loop1;
	st_Vector		vec[100];
};
struct st_Info3
{
	__int32		Loop1;
	st_Vector3F		vec[100];
	__int32		Loop2;
	st_Vector		ID[100];
};
struct st_CTS_ChangePid
{
	__int32		pid;
};
struct st_CTS_LoopBack
{
	__int32		zone;
	__int64		data;
};
struct st_STC_ChangePid
{
	__int32		ret;
};
struct st_STC_LoopBack
{
	__int32		ret;
	__int32		zone;
	__int64		data;
};
