#include "headers/screen.hpp"
#include "headers/shed_error.hpp"

// ONLY for info_bar window
static const size_t num_of_msg = 5;

static const char *keybindings_info[] = {
    "^q = QUIT",
    "^s = SAVE",
    "^d = DELETE LINE",
    "^v = COPY",
    "^p = PASTE",
};


Screen::Screen()
{
    // Lines of infobar
    nlines = 6;

    if (!initscr())
        throw Error(Messages::Code::Init);
 
    if (!has_colors())
        throw Error(Messages::Code::Color);
    else
        start_color();
    
    raw();
    noecho(); 
    halfdelay(1);
    keypad(stdscr, true);
    refresh();

    getmaxyx(stdscr, max_y, max_x);
    right_padding = max_x - 1;
    bottom_padding = max_y - 6;


    textField = new TextField();
    info_bar = newwin(nlines, right_padding, bottom_padding, 0); 
    
    drawInfoBar();
}

Screen::~Screen()
{
    delwin(info_bar);
    endwin();
}

void Screen::update()
{
    // Firstly we update all subwindows of our main window - TextField
    touchwin(textField->getWindow());
    textField->update();
}

void Screen::displayFile(File &file)
{
    wprintw(textField->getWindow(), "%ls", file.getBuffer());
    textField->setRows(file.getRowCount());
    wmove(textField->getWindow(), 0, 0);
}

void Screen::addToBuffer(wchar_t *buffer, const uint32_t row)
{
    uint32_t size;
    wchar_t str[BUFSIZ];
    mvwinwstr(textField->getWindow(), row, 0, str);
    size = textField->getLengthOfString(row);
    str[size + 1] = '\n';
    str[size + 2] = '\0';
    wcscat(buffer, str);
}

wchar_t *Screen::getTFBuffer() 
{
    uint32_t current_row = 0;
    
    uint32_t cur_y = textField->getCurY();
    uint32_t cur_x = textField->getCurX();

    wchar_t *buffer; 
    if (textField->getRows() == 0) {
        buffer = (wchar_t*)calloc(BUFSIZ, sizeof(wchar_t));
        wmove(textField->getWindow(), 0, 0);
        addToBuffer(buffer, current_row);
        return buffer;
    } else {
        buffer = (wchar_t*)calloc(BUFSIZ * textField->getRows(), sizeof(wchar_t));
    }

    wmove(textField->getWindow(), 0, 0);
    // TODO: comment here
    for (; current_row < textField->getRows() + 1; ++current_row)
        addToBuffer(buffer, current_row);
     
    wmove(textField->getWindow(), cur_y, cur_x);
    return buffer;
}


void Screen::drawInfoBar()
{
    uint32_t spacing = 4;
    uint32_t y = 1;
    uint32_t i = 0;

    uint32_t x = max_x / 2 - sizeof(keybindings_info[0]) * spacing;
    
    spacing = 3;
    while (i < num_of_msg) {
        mvwaddstr(info_bar, y, x, keybindings_info[i]);
        y += 2;
        ++i;
        if (y == nlines - 1) {
            x += sizeof(keybindings_info[i]) * 3;
            y = 1;
        }
    }
    box(info_bar, 0, 0);
    wrefresh(info_bar);
}
