#include "headers/shed_error.hpp"

static const char *error_messages[] = {
    "Coudnln't initialize shed.\n",
    "Terminal don't have colors.\n"
    "Can't open file.\n"
    "Can't create file.\n"
};

Error::Error(const Messages::Code err_code)
{
    code = err_code;
    msg = strdup(error_messages[err_code]);
}

char &Error::getMsg()
{
    return *msg;
}

Messages::Code Error::getCode()
{
    return code;
}
