#pragma once
#include <ncurses.h>

#include "shed_error.hpp"
#include "file.hpp"


class TextField {
    friend class InputHandler;    

    uint32_t cursor_y, cursor_x;
    uint32_t cur_win_y, scroll_position;

    uint32_t rows, cols;
    uint32_t max_x, max_y;
    
    uint32_t bottom_padding;
    uint32_t right_padding;

    WINDOW *win;
public:
    TextField();
    ~TextField();

    void update()
    { prefresh(win, scroll_position, 0, 0, 0, bottom_padding, right_padding); }
    void scrollWindow();

    void proccesInput(const cchar_t *input);

    uint32_t getLengthOfString(const uint32_t row);
    uint32_t getRows() const
    { return rows; }
    uint32_t getCurY() const
    { return cursor_y; }
    uint32_t getCurX() const
    { return cursor_x; }
    WINDOW *getWindow() const
    { return win; }

    void setRows(const uint32_t row_count)
    { rows = row_count; }
private:
    void moveToEOL();
};

class Screen {
    uint32_t max_y, max_x; 

    uint32_t nlines;

    uint32_t right_padding;
    uint32_t bottom_padding;

    TextField *textField;
    WINDOW *info_bar;
public:
    Screen();
    ~Screen();

    void update();

    void displayFile(File &file);
       
    // Get TextField buffer
    wchar_t *getTFBuffer();
    TextField &getTF() const
    { return *textField; };
private:
    void addToBuffer(wchar_t *buffer, const uint32_t row);
    void drawInfoBar();
};
