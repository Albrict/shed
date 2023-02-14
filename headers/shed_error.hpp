#pragma once
#include <string.h>
#include <stdlib.h>

namespace Messages {
    enum Code {
        Init,
        Color,
        File,
        Access
    };
};

class Error {
    char *msg;
    Messages::Code code;
public:
    Error(const Messages::Code err_code);
    
    char &getMsg();
    Messages::Code getCode();
};
