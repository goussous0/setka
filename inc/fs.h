#pragma once 
#ifndef FS_H
#define FS_H

#include "base.h"

void list_files();


struct flst
{
    wchar_t fpath[2048];
    struct flst * next;  
};


#endif 