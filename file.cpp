#include "headers/file.hpp"

#include <string.h>
#include <unistd.h>

File::File(char *path)
{
    // If file exist - open it, else - create it
    f = nullptr;
    File::path = path;
    if (!openFD())
        throw Error(Messages::Code::File);
    stat(path, &st);

    uint32_t size = st.st_size;
    row_count = 0; 
    buffer = (wchar_t*)malloc(size * sizeof(wchar_t));

    closeFD();
}

File::~File()
{
    if (buffer)
        free(buffer);
}

void File::readFile()
{
    if (!openFD())
        throw Error(Messages::Code::Access);
    wchar_t str[BUFSIZ];
    
    while (fgetws(str, BUFSIZ, f)) {
        wcscat(buffer, str);
        ++row_count;
    }
    if (row_count < 2) {
        closeFD();
        return;
    } else {
        row_count -= 1;
        closeFD();
    }
}

void File::writeToFile(wchar_t *buffer)
{
    f = fopen(path, "w");
    fprintf(f, "%ls", buffer);
    free(buffer);
    closeFD();
}

bool File::openFD()
{
    
    if (checkFD()) {
        if ((f = fopen(path, "r")) == nullptr) {
            if ((f = fopen(path, "w")) == nullptr) {
                return false;
            }
        }
    }
    return true;
}
