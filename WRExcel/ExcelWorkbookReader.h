#pragma once

#include "xlsxio_read.h"
#include <vector>

class ExcelWorkbookReader
{
public:
    ExcelWorkbookReader();
    virtual ~ExcelWorkbookReader();

    ExcelWorkbookReader(const ExcelWorkbookReader&) = delete;
    ExcelWorkbookReader& operator=(const ExcelWorkbookReader&) = delete;

protected:
    bool OpenWorkbook(const char* fileName);
    void CloseWorkbook();

    xlsxioreader workbook_ = nullptr;
    std::vector<char> workbookData_;
};
