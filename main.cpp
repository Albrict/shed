#include "headers/shed.hpp"

int main (int argc, char **argv)
{
    setlocale(LC_ALL, "");
    
    init(argc, argv);
    run();
    quit();
//    Shed shed(argc, argv);
//    shed.run();
    return 0;
}
