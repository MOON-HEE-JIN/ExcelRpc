#pragma once


#define PROCESS_FROM_FILEHANDLE
#include <corecrt_io.h>
#pragma warning(disable : 4996)

#include <stdlib.h>
#include <stdio.h>
#include "Def.h"
#include <iostream>

#include <windows.h>

#ifdef PROCESS_FROM_FILEHANDLE
//#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include "xlsxio_read.h"

#if !defined(XML_UNICODE_WCHAR_T) && !defined(XML_UNICODE)
//UTF-8 version
#define X(s) s
#define XML_Char_printf printf
#else
//UTF-16 version
#define X(s) L##s
#define XML_Char_printf wprintf
#endif

class CWRExcel
{
public:
	CWRExcel();
	~CWRExcel();

public:

protected:
	xlsxioreader xlsxioread;
	char SheetName[10][256];
	int SheetCount;

public:
	bool OpenXLSX(const char* FileName);
};

