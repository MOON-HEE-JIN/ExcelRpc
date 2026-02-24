#include "CWRExcel.h"

CWRExcel::CWRExcel()
{
	memset(SheetName, 0, 10 * 256);
	SheetCount = 0;
	xlsxioread = nullptr;

	SetConsoleOutputCP(CP_UTF8);

	XML_Char_printf("XLSX I/O library bersion %s\n", xlsxioread_get_version_string());
}

CWRExcel::~CWRExcel()
{
}

bool CWRExcel::OpenXLSX(const char* FileName)
{
	int filehandle;
	
	if ((filehandle = open(FileName, O_RDONLY | O_BINARY, 0)) == -1)
	{
		fprintf(stderr, "ERROR OPEN %s.xlsx FILE\n", FileName);
		return false;
	}

	if ((xlsxioread = xlsxioread_open_filehandle(filehandle)) == NULL)
	{
		fprintf(stderr, "ERROR READ %s.xlsx FILE\n", FileName);
		return false;
	}

	
	//list available sheets
	xlsxioreadersheetlist sheetlist;
	const XLSXIOCHAR* sheetname;
	printf("Available sheets:\n");

	if ((sheetlist = xlsxioread_sheetlist_open(xlsxioread)) != NULL) {
		while ((sheetname = xlsxioread_sheetlist_next(sheetlist)) != NULL) {
			XML_Char_printf(X("==== SheetName ==== %s\n"), sheetname);
			memcpy(&SheetName[SheetCount], sheetname, strlen(sheetname));

			SheetCount++;
		}
		xlsxioread_sheetlist_close(sheetlist);
	}
	
	return true;
}
