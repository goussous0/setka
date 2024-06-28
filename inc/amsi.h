#pragma once

#ifndef ASMI_H
#define ASMI_H

#include "fs.h"

DECLARE_HANDLE(HAMSICONTEXT);
DECLARE_HANDLE(HAMSISESSION);

typedef enum AMSI_RESULT
{
    AMSI_RESULT_CLEAN = 0,
    AMSI_RESULT_NOT_DETECTED = 1,
    AMSI_RESULT_BLOCKED_BY_ADMIN_START = 0x4000,
    AMSI_RESULT_BLOCKED_BY_ADMIN_END = 0x4fff,
    AMSI_RESULT_DETECTED = 32768
} AMSI_RESULT;

typedef HRESULT(WINAPI * amsi_open_session_func)(
    HAMSICONTEXT amsiContext,
    HAMSISESSION *amsiSession
);

typedef HRESULT(WINAPI * amsi_scan_buffer_func)(
    HAMSICONTEXT amsiContext,
    PVOID buffer,
    ULONG length,
    LPCWSTR contentName,
    HAMSISESSION amsiSession,
    AMSI_RESULT *result
);

typedef HRESULT (WINAPI * amsi_scan_string_func)(
    HAMSICONTEXT amsiContext,
    LPCWSTR      string,
    LPCWSTR      contentName,
    HAMSISESSION amsiSession,
    AMSI_RESULT  *result
);


typedef HRESULT (WINAPI * amsi_init_func)(
    LPCWSTR      appName,
    HAMSICONTEXT *amsiContext
);

typedef void (WINAPI * amsi_uninit_func)(
    HAMSICONTEXT amsiContext
);


void scan_files();
int load_amsi();

void scan_file();

int merge_scan();
int locate_badchar();

#endif 