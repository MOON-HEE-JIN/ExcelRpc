#include "ExcelWorkbookReader.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>

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
    // Read through a Unicode filesystem path; the ZIP library's narrow path
    // handling cannot reliably open Korean filenames on Windows.
    std::ifstream input(std::filesystem::u8path(fileName), std::ios::binary);
    if (input)
    {
        workbookData_.assign(std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>());
        if (!input.bad() && !workbookData_.empty())
            workbook_ = xlsxioread_open_memory(
                workbookData_.data(), workbookData_.size(), 0);
    }
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
    workbookData_.clear();
}
