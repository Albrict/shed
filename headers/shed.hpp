#pragma once
#include "screen.hpp"
#include "input_handler.hpp"

#include <stdio.h>
#include <locale.h>

class Shed {
    Screen *m_screen;
    File *m_file;
    InputHandler inpHandler;

    bool m_running; 

public:
    Shed(int argc, char **argv) noexcept;
    ~Shed();

    void run() noexcept;
private:
    void proccesEvents() noexcept;

    void die(Error &ex) noexcept;
    void panic(Error &ex) noexcept;
};
