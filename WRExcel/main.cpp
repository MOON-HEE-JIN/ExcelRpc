
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

#include <map>
#include <list>
#include <string>


#include "CWRExcel.h"
#include "CWRStruct.h"
#include "CMSVCRPC.h"
#include "CCshapRPC.h"

int main(int argc, char* argv[])
{
    if (1)
    {
        CMSVCRPC t;
        //t.MSVC_ALL_FILE();

        g_CWRRPCManager.Init("RPC_GAME_EXCEL.xlsx");
        //g_CWRRPCManager.Init("RPC_OBSERVER_EXCEL.xlsx");
        //"RPC_GAME_EXCEL.xlsx"
        //RPC_CHAT_EXCEL
        //t.SetCustomName("Observer_");
        t.MSVC_ALL_FILE();

        //CCshapRPC tt;
        //tt.CSHAP_ALL_FILE();
        
        system("pause");
        return 0;
   }
    printf("END\n\n");
    return 0;
}

