#include "headers/shed.hpp"
#include "headers/event.hpp"
#include "headers/file.hpp"

Shed::Shed(int argc, char **argv) noexcept
{
    m_running = true;
    try {
        m_screen = new Screen(); 
        if (argc > 1) {
            m_file = new File(argv[1]);
            m_file->readFile();
        } else {
            endwin();
            exit(0);
        }
    } catch (Error &ex) {
        die(ex);
    }
    m_screen->displayFile(*m_file);

    set_tabsize(4);
}

Shed::~Shed()
{
    m_file->writeToFile(m_screen->getTFBuffer());

    delete m_screen;
    delete m_file;
}

void Shed::proccesEvents() noexcept
{
    wint_t input;
    cchar_t output;
    EventCode::Code code;     
    
    int success;
    success = get_wch(&input);

    if (success == ERR)
        return;
    setcchar(&output, (wchar_t*)&input, 0, 0, nullptr);

    inpHandler.proccesInput(m_screen->getTF(), &output); 

    code = Event::getInstance()->getEvent();
    switch(code) {
    case EventCode::Code::Quit:
        m_running = false;
        break;
    case EventCode::Code::Save: 
        m_file->writeToFile(m_screen->getTFBuffer()); 
        break;
    };
}

void Shed::run() noexcept
{
    while (m_running) {
        proccesEvents();
        m_screen->update();
    }
}

void Shed::die(Error &ex) noexcept
{
    endwin();
    fprintf(stderr, "Error message: %s", &ex.getMsg());    
    fprintf(stderr, "Error code: %d\n", ex.getCode());

    exit_curses(ex.getCode());
}

void Shed::panic(Error &ex) noexcept
{
    fprintf(stderr, "Error message: %s", &ex.getMsg());    
    fprintf(stderr, "Error code: %d\n", ex.getCode());
}
