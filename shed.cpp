#include "headers/shed.hpp"
#include "headers/event.hpp"
#include "headers/file.hpp"

static bool g_running = true;
static Screen *g_screen;
static File *g_file;
static InputHandler g_inputHandler;

static void die(Error &ex)
{
    endwin();
    fprintf(stderr, "Error message: %s", &ex.getMsg());    
    fprintf(stderr, "Error code: %d\n", ex.getCode());

    exit_curses(ex.getCode());
}

//static void panic(Error &ex)
//{
//    fprintf(stderr, "Error message: %s", &ex.getMsg());    
//    fprintf(stderr, "Error code: %d\n", ex.getCode());
//}

static void proccesEvents()
{
    wint_t input;
    cchar_t output;
    EventCode::Code code;     
    
    int success;
    success = get_wch(&input);

    if (success == ERR)
        return;
    setcchar(&output, (wchar_t*)&input, 0, 0, nullptr);

    g_inputHandler.proccesInput(g_screen->getTF(), &output); 

    code = Event::getInstance()->getEvent();
    switch(code) {
    case EventCode::Code::Quit:
        g_running = false;
        break;
    case EventCode::Code::Save: 
        g_file->writeToFile(g_screen->getTFBuffer()); 
        break;
    };
}

void quit() noexcept
{
    g_file->writeToFile(g_screen->getTFBuffer());

    delete g_screen;
    delete g_file;
}

void run() noexcept
{
    while (g_running) {
        proccesEvents();
        g_screen->update();
    }
}

void init(int argc, char **argv) noexcept
{
    g_running = true;
    try {
        g_screen = new Screen(); 
        if (argc > 1) {
            g_file = new File(argv[1]);
            g_file->readFile();
        } else {
            endwin();
            exit(0);
        }
    } catch (Error &ex) {
        die(ex);
    }
    g_screen->displayFile(*g_file);

    set_tabsize(4);
}
