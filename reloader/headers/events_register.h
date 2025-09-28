#ifndef EVENTSREGISTER_H
#define EVENTSREGISTER_H

#include <iostream>
#include <atomic>

class events_register{
    private:
        std::atomic<bool> event;
        events_register();
        events_register(const events_register&) = delete;
        events_register& operator=(const events_register&) = delete;
    public:
        bool  read_event();
        void add_event();
        static events_register& getInstance(){
            static events_register instance;
            return instance;
        }
};

#endif // EVENTSREGISTER_H