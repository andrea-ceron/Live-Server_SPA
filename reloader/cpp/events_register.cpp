#include "../headers/events_register.h"

void events_register::add_event(){
    event = true;
}

bool events_register::read_event(){
    return event.exchange(false);
}

events_register::events_register() {

}