#include "../include/system/SysEvent.h"

std::ostream &operator<<(std::ostream &os, const SysEvent &event) {
    if (event.id == SysEvent::EVENT_13_ERROR)
        os << *event.time.get() << " " << event.id << " " << event.msgError << std::endl;

    else if (event.id == SysEvent::EVENT_12_SIT_DOWN)
        os << *event.time.get() << " " << event.id << " " << event.client << " " << event.table << std::endl;

    else if (event.id == SysEvent::EVENT_11_KICKED_OUT)
        os << *event.time.get() << " " << event.id << " " << event.client << std::endl;

    else if (event.id == SysEvent::EVENT_1_CAME)
        os << *event.time.get() << " " << event.id << " " << event.client << std::endl;

    else if (event.id == SysEvent::EVENT_2_CHANGE_SEAT)
        os << *event.time.get() << " " << event.id << " " << event.client << " " << event.table << std::endl;

    else if (event.id == SysEvent::EVENT_3_WAIT)
        os << *event.time.get() << " " << event.id << " " << event.client << std::endl;

    else if (event.id == SysEvent::EVENT_4_GONE)
        os << *event.time.get() << " " << event.id << " " << event.client << std::endl;

    else
        os << *event.time << " " << event.id << " " << event.client << " " << event.table << std::endl;

    return os;
}

int SysEvent::getId() const {
    return id;
}

void SysEvent::setId(int id) {
    SysEvent::id = id;
}

const std::string &SysEvent::getClient() const {
    return client;
}

void SysEvent::setClient(const std::string &client) {
    SysEvent::client = client;
}

int SysEvent::getTable() const {
    return table;
}

void SysEvent::setTable(int table) {
    SysEvent::table = table;
}

const std::string &SysEvent::getMsgError() const {
    return msgError;
}

void SysEvent::setMsgError(const std::string &msgError) {
    SysEvent::msgError = msgError;
}

bool SysEvent::isSpoiled() const {
    return spoiled;
}

void SysEvent::setSpoiled(bool spoiled) {
    SysEvent::spoiled = spoiled;
}

const std::shared_ptr<Time> &SysEvent::getTime() const {
    return time;
}

void SysEvent::setTime(const std::shared_ptr<Time> &time) {
    SysEvent::time = time;
}


SysEvent::SysEvent(int id, const std::shared_ptr<Time> &time, const std::string &client) : id(id), time(time), client(client) {}

SysEvent::SysEvent(int id, const std::shared_ptr<Time> &time, const std::string &client, int table) : id(id), time(time), client(client),
                                                                               table(table) {}

SysEvent::SysEvent(int id, const std::shared_ptr<Time> &time, const std::string &msgError, bool spoiled) : id(id), time(time),
                                                                                    msgError(msgError),
                                                                                    spoiled(spoiled) {}
