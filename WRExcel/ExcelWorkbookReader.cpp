#include "ExcelWorkbookReader.h"

#include <cstdio>

#include <windows.h>

ExcelWorkbookReader::ExcelWorkbookReader()
{
    SetConsoleOutputCP(CP_UTF8);
    std::printf("XLSX I/O library version %s\n", xlsxioread_get_version_string());
}

ExcelWorkbookReader::~ExcelWorkbookReader()
{
    CloseWorkbook();
}

bool ExcelWorkbookReader::OpenWorkbook(const char* fileName)
{
    CloseWorkbook();
    workbook_ = xlsxioread_open(fileName);
    if (workbook_ != nullptr)
    {
        return true;
    }

    std::fprintf(stderr, "Failed to open XLSX workbook: %s\n", fileName);
    return false;
}

void ExcelWorkbookReader::CloseWorkbook()
{
    if (workbook_ == nullptr)
    {
        return;
    }

    xlsxioread_close(workbook_);
    workbook_ = nullptr;
}
