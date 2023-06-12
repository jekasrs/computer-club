#include "../include/objects/Client.h"

Client::Client(const std::string &name) : name(name) {}

const std::string &Client::getName() const {
    return name;
}

void Client::setName(const std::string &name) {
    Client::name = name;
}

bool Client::isHere() const {
    return isInHere;
}

void Client::setIsHere(bool isHere) {
    isInHere = isHere;
}

const std::shared_ptr<SysEvent>& Client::getCurrentEvent() const {
    return currentEvent;
}

void Client::setCurrentEvent(const SysEvent &event) {
    Client::currentEvent = std::make_unique<SysEvent>(event);
}
