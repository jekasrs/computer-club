#ifndef MY_SYSTEM_SYSEVENT_H
#define MY_SYSTEM_SYSEVENT_H

#include <string>
#include <ostream>

#include "../objects/Time.h"

class SysEvent {

public:
    SysEvent(int id, const std::shared_ptr<Time> &time, const std::string &client);
    SysEvent(int id, const std::shared_ptr<Time> &time, const std::string &client, int table);
    SysEvent(int id, const std::shared_ptr<Time> &time, const std::string &msgError, bool spoiled);

    int getId() const;
    void setId(int id);

    const std::string &getClient() const;
    void setClient(const std::string &client);

    int getTable() const;
    void setTable(int table);

    const std::shared_ptr<Time> &getTime() const;
    void setTime(const std::shared_ptr<Time> &time);

    const std::string &getMsgError() const;
    void setMsgError(const std::string &msgError);

    bool isSpoiled() const;
    void setSpoiled(bool spoiled);

    static inline constexpr auto EVENT_1_CAME = 1;
    static inline constexpr auto EVENT_2_CHANGE_SEAT = 2;
    static inline constexpr auto EVENT_3_WAIT = 3;
    static inline constexpr auto EVENT_4_GONE = 4;
    static inline constexpr auto EVENT_11_KICKED_OUT = 11;
    static inline constexpr auto EVENT_12_SIT_DOWN = 12;
    static inline constexpr auto EVENT_13_ERROR = 13;

    friend std::ostream &operator<<(std::ostream &os, const SysEvent &event);

private:
    int id;
    std::shared_ptr<Time> time;
    std::string client;
    int table;

    std::string msgError;
    bool spoiled = false;
};

#endif
