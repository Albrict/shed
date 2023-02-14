namespace EventCode {
    enum Code {
        Quit,
        Save
    };
};

class Event {
    EventCode::Code code;
public:
    static Event *getInstance() {
        if (!instance) {
            instance = new Event();
            return instance; 
        }
        return instance;
    }

    void setEvent(const EventCode::Code code)
    { Event::code = code; }
    EventCode::Code getEvent()
    { return code; }
private:
    static Event *instance;
    Event() {}
    Event(const Event&);
    Event& operator=(Event&);
};
