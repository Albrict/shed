#pragma once
#include <stdio.h>
#include <ncurses.h>

#include "screen.hpp"

enum class mode { insert = 0, edit, copy };
enum keys { key_esc = 27 };

class InputHandler {
    mode m_currentMode;
    wchar_t m_copyBuffer[BUFSIZ];
public:
    InputHandler() : m_currentMode(mode::edit) {}
    void proccesInput(TextField &tf, cchar_t *input) noexcept;
private:
    void proccesInsert(TextField &tf, const cchar_t *input) noexcept;
    void proccesEdit(TextField &tf, const cchar_t *input) noexcept; 
    void proccesCopy(TextField &tf, const cchar_t *input) noexcept;

    void insertRow(TextField &tf) noexcept;
    void deleteRow(TextField &tf) noexcept;

    void insertChar(TextField &tf, const cchar_t *ch) noexcept;
    void insertTab(TextField &tf) noexcept;
    void deleteChar(TextField &tf) noexcept;

    void moveUp(TextField &tf) noexcept;
    void moveDown(TextField &tf) noexcept;
    void moveLeft(TextField &tf) noexcept;
    void moveRight(TextField &tf) noexcept;

    void moveRightTill(TextField &tf) noexcept;
    void moveLeftTill(TextField &tf) noexcept;

    void copyChar(TextField &tf, const uint32_t buf_pos) noexcept;
    void paste(TextField &tf) noexcept
    { mvwins_wstr(tf.win, tf.cursor_y, tf.cursor_x, m_copyBuffer); }
};
