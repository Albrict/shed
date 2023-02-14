#include "headers/shed.hpp"

int main (int argc, char **argv)
{
    setlocale(LC_ALL, "");
    Shed shed(argc, argv);
    shed.run();
    return 0;
}
