#ifndef MY_SYSTEM_CLIENT_H
#define MY_SYSTEM_CLIENT_H

#include <string>
#include "../system/SysEvent.h"

class Client {
public:
    Client(const std::string &name);

    const std::string &getName() const;

    void setName(const std::string &name);

    bool isHere() const;

    void setIsHere(bool isHere);

    const std::shared_ptr<SysEvent>& getCurrentEvent() const;

    void setCurrentEvent(const SysEvent &event);

private:
    std::string name;
    bool isInHere;
    std::shared_ptr<SysEvent> currentEvent;

};

#endif
