#include "fs.h"
#include <stddef.h>



void list_files(const wchar_t * path, struct flst * files)
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = NULL; 
    wchar_t search_path[2048]; 

    wsprintf(search_path, L"%s\\*.*", path);

    if((hFind = FindFirstFile(search_path, &ffd)) == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Path %s not found\n", path);
        return; 
    }

    do
    {
        if (wcscmp(ffd.cFileName, L".") != 0 &&
            wcscmp(ffd.cFileName, L"..") != 0)
        {
            if (ffd.dwFileAttributes ^ FILE_ATTRIBUTE_DIRECTORY)
            {
                // wprintf(L"%S\n", ffd.cFileName);
                wsprintf(files->fpath, L"%s\\%s", path, ffd.cFileName);
                files->next = malloc(sizeof(struct flst));
                files = files->next;
            }
        }


    } while (FindNextFile(hFind, &ffd));
    FindClose(hFind);
    files->next = NULL;
    return;
}