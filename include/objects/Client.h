#ifndef MY_SYSTEM_CLIENT_H
#define MY_SYSTEM_CLIENT_H

#include <string>
#include "../system/SysEvent.h"

class Client {
public:
    explicit Client(const std::string &name) noexcept;

    const std::string &getName() const noexcept;

    bool isHere() const noexcept;

    void setIsHere(bool isHere) noexcept;

    static bool compareByName(const std::shared_ptr<Client> &c1, const std::shared_ptr<Client> &c2);

private:
    const std::string name;
    bool isInHere;

};

#endif
