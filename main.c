#include "amsi.h"


int main(int argc, char * argv[])
{
    if (load_amsi() != 0)
    {
        return 1;
    }

    struct flst files;
    list_files(L"C:\\Users\\malware\\Desktop\\setka\\samples", &files);
    // scan_files(&files);


    // scan_file(L"C:\\Users\\malware\\Desktop\\setka\\samples\\msg2.exe");
    scan_file(L"C:\\Users\\malware\\Desktop\\shell_code\\build\\app.exe");




    // for (struct flst * tmp = &files; tmp->next != NULL; tmp = tmp->next)
    // {
    //     scan_file(tmp->fpath);
    // }


    return 0;
}
