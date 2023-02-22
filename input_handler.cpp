#include "headers/input_handler.hpp"
#include "headers/event.hpp"

#define CTRL_KEY(k) ((k) & 0x1f)

void InputHandler::proccesInput(TextField &tf, cchar_t *input) noexcept
{
    switch(m_currentMode) {
    case mode::insert:
        proccesInsert(tf, input);
        break;
    case mode::edit:
        proccesEdit(tf, input);
        break;
    case mode::copy:
        proccesCopy(tf, input);
        break;
    }
}

void InputHandler::proccesInsert(TextField &tf, const cchar_t *input) noexcept
{
    switch(*input->chars) {

    // Keybinding           // Method
    case keys::key_esc:     m_currentMode = mode::edit;     break;
    case CTRL_KEY('q'):     Event::getInstance()->setEvent(EventCode::Code::Quit);      break;
    case CTRL_KEY('d'):     deleteRow(tf);                  break;
    case CTRL_KEY('s'):     Event::getInstance()->setEvent(EventCode::Code::Save);      break;
    case CTRL_KEY('v'):     m_currentMode = mode::copy;     break;
    case CTRL_KEY('p'):     paste(tf);                      break;

    case KEY_UP:            moveUp(tf);                     break;
    case KEY_DOWN:          moveDown(tf);                   break;
    case KEY_LEFT:          moveLeft(tf);                   break;
    case KEY_RIGHT:         moveRight(tf);                  break;

    case KEY_BACKSPACE: 
    case KEY_DL: 
    case 127:               deleteChar(tf);                 break;

    case '\n':              insertRow(tf);                  break;
    case '\t':              insertTab(tf);                  break;
    default:                insertChar(tf, input);          break;

    }
}

void InputHandler::proccesEdit(TextField &tf, const cchar_t *input) noexcept
{
    switch(*input->chars) {

    // Keybinding           // Method
    case CTRL_KEY('q'):     Event::getInstance()->setEvent(EventCode::Code::Quit);      break;
    case CTRL_KEY('d'):     deleteRow(tf);                  break;
    case CTRL_KEY('s'):     Event::getInstance()->setEvent(EventCode::Code::Save);      break;
    case CTRL_KEY('v'):     m_currentMode = mode::copy;     break;
    case CTRL_KEY('p'):     paste(tf);                      break;

    case KEY_UP:            moveUp(tf);                     break;
    case KEY_DOWN:          moveDown(tf);                   break;
    case KEY_LEFT:          moveLeft(tf);                   break;
    case KEY_RIGHT:         moveRight(tf);                  break;

    case 'p':               paste(tf);                      break;
    case 'h':               moveLeft(tf);                   break;
    case 'j':               moveDown(tf);                   break;
    case 'k':               moveUp(tf);                     break;
    case 'l':               moveRight(tf);                  break;
    case 'w':               moveRightTill(tf);              break;
    case 'b':               moveLeftTill(tf);               break;
    case 'x':               deleteCharInEditMode(tf);       break;
    case 'a':               m_currentMode = mode::insert;   break;
    case 'A':               moveToEndAndInsert(tf);         break;
    case 'i':               startInsert(tf);                break;
    default:
        return;
        break;
    }
}

void InputHandler::startInsert(TextField &tf)
{
    ++tf.cursor_x;
    wmove(tf.win, tf.cursor_y, tf.cursor_x);
    m_currentMode = mode::insert;
}

void InputHandler::proccesCopy(TextField &tf, const cchar_t *input) noexcept
{
    static uint32_t pos = 0;
    
    switch(*input->chars) {
    case KEY_RIGHT:
        moveRight(tf);
        copyChar(tf, pos);
        ++pos;
        break;
    case 'l':
        moveRight(tf);
        copyChar(tf, pos);
        ++pos;
        break;
    case 'y':
        m_copyBuffer[pos + 1] = '\0';
        m_currentMode = mode::edit;
        break;
    }
}

void InputHandler::insertRow(TextField &tf) noexcept
{
    tf.cols = 1;
    ++tf.rows;

    tf.cursor_x = 0;
    ++tf.cursor_y;
    ++tf.cur_win_y;

    wmove(tf.win, tf.cursor_y, tf.cursor_x);
    winsertln(tf.win);
    tf.scrollWindow();
}

void InputHandler::deleteRow(TextField &tf) noexcept
{
    if (tf.cursor_y == 0 && tf.cursor_x == 0)
        wclrtoeol(tf.win);
    wdeleteln(tf.win);
    tf.cols = 1;
    --tf.rows;

    --tf.cursor_y;
    --tf.cur_win_y;
    tf.moveToEOL();
    tf.scrollWindow();
}

void InputHandler::insertChar(TextField &tf, const cchar_t *ch) noexcept
{
    if (tf.cursor_x == (uint32_t)getmaxx(stdscr) - 6) {
        ++tf.cursor_y;
        tf.cursor_x = 0;
        wmove(tf.win, tf.cursor_y, tf.cursor_x);
    }
    ++tf.cursor_x;
    ++tf.cols;
    mvwins_wch(tf.win, tf.cursor_y, tf.cursor_x - 1, ch);
    wmove(tf.win, tf.cursor_y, tf.cursor_x);
}

void InputHandler::insertTab(TextField &tf) noexcept
{
    uint32_t x = tf.cursor_x;
    tf.cursor_x += TABSIZE;

    if (tf.cursor_x >= tf.max_x) {
        insertRow(tf);
    } else {
        for (; x < tf.cursor_x; ++x)
            mvwaddch(tf.win, tf.cursor_y, x, ' ');
    }
}

void InputHandler::deleteChar(TextField &tf) noexcept
{
    if (tf.cursor_x == 0 && tf.cursor_y == 0)
        return;
    if (tf.cursor_x == 0 && tf.cursor_y > 0) {
        deleteRow(tf);
        return;
    }
    --tf.cursor_x;
    --tf.cols;
    mvwdelch(tf.win, tf.cursor_y, tf.cursor_x);
}

void InputHandler::moveToEndAndInsert(TextField &tf) noexcept
{
    tf.moveToEOL();
    m_currentMode = mode::insert;
}

void InputHandler::deleteCharInEditMode(TextField &tf) noexcept
{
    mvwdelch(tf.win, tf.cursor_y, tf.cursor_x);
    --tf.cursor_x;
    --tf.cols;
}

void InputHandler::moveUp(TextField &tf) noexcept
{
    if (tf.cursor_y == 0)
        return;
    --tf.cursor_y;
    --tf.cur_win_y;
    tf.scrollWindow();
    
    if (tf.cursor_x < tf.getLengthOfString(tf.cursor_y))
        wmove(tf.win, tf.cursor_y, tf.cursor_x);
    else
        tf.moveToEOL();
}

void InputHandler::moveDown(TextField &tf) noexcept
{
    if (tf.cursor_y == tf.rows)
        return;
    ++tf.cursor_y;
    ++tf.cur_win_y;
    tf.scrollWindow();

    if (tf.cursor_x < tf.getLengthOfString(tf.cursor_y))
        wmove(tf.win, tf.cursor_y, tf.cursor_x);
    else
        tf.moveToEOL();
}

void InputHandler::moveLeft(TextField &tf) noexcept
{
    if (tf.cursor_y == 0 && tf.cursor_x == 0) {
        return;
    }
    if (tf.cursor_y > 0 && tf.cursor_x == 0) {
        --tf.cursor_y;
        tf.moveToEOL();
        return;
    }
    --tf.cursor_x; 
    wmove(tf.win, tf.cursor_y, tf.cursor_x);
}

void InputHandler::moveRight(TextField &tf) noexcept
{
    uint32_t length = tf.getLengthOfString(tf.cursor_y);

    if (tf.cursor_x == length) {
        if (tf.cursor_y == tf.rows)
            return;
        ++tf.cursor_y;
        tf.cursor_x = 0;
        wmove(tf.win, tf.cursor_y, tf.cursor_x);
        return;
    }
    if (tf.cursor_x < length) {
        ++tf.cursor_x; 
        wmove(tf.win, tf.cursor_y, tf.cursor_x);
    }
}

void InputHandler::moveRightTill(TextField &tf) noexcept
{
    if (tf.cursor_x == tf.getLengthOfString(tf.cursor_y) && tf.cursor_y == tf.rows)
        return;

    if ((winch(tf.win) & A_CHARTEXT) == ' ') {
        while ((winch(tf.win) & A_CHARTEXT)  == ' ')
            moveRight(tf);
    } else {
        while((winch(tf.win) & A_CHARTEXT) != ' ')
            moveRight(tf);
    }
}

void InputHandler::moveLeftTill(TextField &tf) noexcept
{
    if ((winch(tf.win) & A_CHARTEXT) == ' ') {
        while ((winch(tf.win) & A_CHARTEXT)  == ' ')
            moveLeft(tf);
    } else {
        while((winch(tf.win) & A_CHARTEXT) != ' ') {
            moveLeft(tf);
            if (tf.cursor_x == 0)
                return;
        }
    }
}

void InputHandler::copyChar(TextField &tf, const uint32_t buf_pos) noexcept
{
    cchar_t ch;
    mvwin_wch(tf.win, tf.cursor_y, tf.cursor_x, &ch);

    if (m_copyBuffer[buf_pos] == *ch.chars)
        return;
    else
        m_copyBuffer[buf_pos] = *ch.chars;  
}

void InputHandler::paste(TextField &tf) noexcept
{
    mvwins_wstr(tf.win, tf.cursor_y, tf.cursor_x, m_copyBuffer);
}
