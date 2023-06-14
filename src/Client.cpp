#include "../include/objects/Client.h"

Client::Client(const std::string &name) noexcept : name(name) {}

const std::string &Client::getName() const noexcept{
    return name;
}

bool Client::isHere() const noexcept {
    return isInHere;
}

void Client::setIsHere(bool isHere) noexcept {
    isInHere = isHere;
}

bool Client::compareByName(const std::shared_ptr<Client>& c1, const std::shared_ptr<Client>& c2) {
    return c1->getName() < c2->getName();
}
