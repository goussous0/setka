#include "amsi.h"

HMODULE amsi = NULL;
amsi_open_session_func amsi_open_session;
amsi_scan_buffer_func  amsi_scan_buffer;
amsi_scan_string_func  amsi_scan_string;
amsi_init_func amsi_init;
amsi_uninit_func amsi_uninit;

HAMSICONTEXT ctx;
HAMSISESSION session;

void scan_files(struct flst * files)
{
    for (; files->next != NULL; files = files->next)
    {
        wprintf(L"%S\n", files->fpath);
        HANDLE ptr; 
        DWORD size;
        wchar_t bytes[1000000];
        ptr = CreateFile(files->fpath, GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
        if (ptr == INVALID_HANDLE_VALUE)
        {
            wprintf(L"failed to open %S\n", files->fpath);
        }
        ReadFile(ptr, bytes, 1000000, &size, NULL);
        wprintf(L"file size %d\n", size);

        AMSI_RESULT res; 

        amsi_scan_buffer(ctx, bytes, size, files->fpath, NULL, &res);
        wprintf(L"AMSI_result: %d\n\n", res);


        CloseHandle(ptr);


    }

}

int load_amsi()
{
    amsi = LoadLibrary(L"amsi.dll");
    if (amsi == NULL)
    {
        wprintf(L"Failed to load AMSI\n");
        return -1;
    }

    amsi_scan_buffer = (amsi_scan_buffer_func)GetProcAddress(amsi, "AmsiScanBuffer");
    if (amsi_scan_buffer == NULL)
    {
        wprintf(L"Failed to find amsi_scan_buffer\n");
        return -1;
    }

    amsi_scan_string = (amsi_scan_string_func)GetProcAddress(amsi, "AmsiScanString");
    if (amsi_scan_string == NULL)
    {
        wprintf(L"Failed to find amsi_scan_string\n");
        return -1;
    }


    amsi_open_session = (amsi_open_session_func)GetProcAddress(amsi, "AmsiOpenSession");
    if (amsi_open_session == NULL)
    {
        wprintf(L"Failed to find amsi_open_session\n");
        return -1;
    }


    amsi_init = (amsi_init_func)GetProcAddress(amsi, "AmsiInitialize");
    if (amsi_init == NULL)
    {
        wprintf(L"Failed to find amsi_init\n");
        return -1;
    }


    amsi_uninit = (amsi_uninit_func)GetProcAddress(amsi, "AmsiUninitialize");
    if (amsi_uninit == NULL)
    {
        wprintf(L"Failed to find amsi_uninit\n");
        return -1;
    }    

    amsi_init(L"setka", &ctx);
    

    return 0;
}


void scan_file(const wchar_t * path)
{
    DWORD size;
    HANDLE ptr;

    ptr = CreateFile(path, GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

    if (ptr == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Failed to open %S\n", path);
    }

    // wchar_t buffer[8192];

    size = GetFileSize(ptr, NULL);
    wprintf(L"File size: %d\n", size);

    wchar_t * buffer = malloc(size * sizeof(wchar_t));

    ReadFile(ptr, buffer, size, &size, NULL);
    wprintf(L"Read size: %d\n", size);
    wprintf(L"%S\n", path);

    ULONG start, end; 
    merge_scan(buffer, 0, size, &start, &end);

    locate_badchar(buffer, start, end);

    // wprintf(L"start: %d\n", start);
    // wprintf(L"start: %d\n", end);
    free(buffer);

}


int merge_scan(wchar_t * buffer, ULONG start, ULONG end, ULONG * s, ULONG * e)
{
    if (end - start < 1) return 0;
    AMSI_RESULT res;
    amsi_scan_buffer(ctx, buffer+start, end-start, L"buffer", NULL, &res);
    if (res == AMSI_RESULT_DETECTED)
    {
        wprintf(L"\tAMSI_result: %d\n", res);
        wprintf(L"\tbuffer size: %d\n", end-start);
        wprintf(L"\tstart: %d\n", start);
        wprintf(L"\tend: %d\n", end);
        wprintf(L"\n");

        *s = start;
        *e = end;

        ULONG mid = (end-start)/2;

        merge_scan(buffer, start, mid, s, e);
        merge_scan(buffer, mid, end, s, e);
    }
    return res;
}


int locate_badchar(wchar_t * buffer, ULONG start, ULONG end)
{
    AMSI_RESULT res;
    for (long i = start; i < end; i++)
    {
        for (long j = start+1; j < end; j++)
        {
            amsi_scan_buffer(ctx, buffer+i, j-i, L"buffer", NULL, &res);
            if (res == AMSI_RESULT_DETECTED)
            {
                wprintf(L"I: %d\n", i);
                wprintf(L"J: %d\n", j);
            }
        }
    }
    return 0;
}