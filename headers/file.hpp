#pragma once
#include "shed_error.hpp"

#include <stdio.h>
#include <stdint.h>
#include <wchar.h>
#include <sys/stat.h>

class File {
    FILE *f;
    struct stat st;

    char *path;
    wchar_t *buffer;

    uint32_t row_count;
public:
    File(char *path);
    ~File();
    
    void readFile();
    void writeToFile(wchar_t *buffer);

    wchar_t *getBuffer() const
    { return buffer; }
    uint32_t getRowCount() const
    { return row_count; }
    uint32_t getFileSize() const 
    {uint32_t size; size = st.st_size; return size; }
private:
    bool openFD();
    void closeFD()
    { fclose(f); f = nullptr; }
    bool checkFD()
    { return f == nullptr ? true : false; }
};
