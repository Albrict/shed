#include "headers/screen.hpp"
#include "headers/event.hpp"

#include <ctype.h>

#define CTRL_KEY(k) ((k) & 0x1f)

static const uint32_t pad_max_y = 10000;

TextField::TextField()
{
    cursor_y = 0;
    cursor_x = 0;

    cols = 1;
    rows = 0;
    
    getmaxyx(stdscr, max_y, max_x);

    bottom_padding = max_y - 7;
    right_padding = max_x - 1; 
    // We make it pad, so we can scroll our window
    win = newpad(pad_max_y, max_x);
    scroll_position = 0;
    scrollok(win, true);
}

TextField::~TextField()
{
    delwin(win);
}

void TextField::scrollWindow()
{
    if (cur_win_y == bottom_padding) {
        ++scroll_position;
        --cur_win_y;
    } else if (cur_win_y == 0 && scroll_position > 0) {
        --scroll_position;
        ++cur_win_y;
    }
}

void TextField::moveToEOL()
{
    cursor_x = getmaxx(stdscr) - 6;
    wmove(win, cursor_y, cursor_x);

    // While there's spaces on screen - move left
    while ((winch(win) & A_CHARTEXT)  == ' ') {
        if (cursor_x == 0)
            return;
        wmove(win, cursor_y, --cursor_x);
    }

    cursor_x++;
    wmove(win, cursor_y, cursor_x);
}

uint32_t TextField::getLengthOfString(const uint32_t row)
{
    uint32_t orig_x = cursor_x;
    uint32_t orig_y = cursor_y;

    uint32_t length;

    if (row > rows)
        return 0;

    cursor_y = row;
    moveToEOL();
    length = cursor_x;

    cursor_x = orig_x;
    cursor_y = orig_y;
    wmove(win, cursor_y, cursor_x);
    return length;
}
