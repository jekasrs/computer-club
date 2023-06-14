#ifndef MY_SYSTEM_DISPATCHER_H
#define MY_SYSTEM_DISPATCHER_H

#include <ostream>
#include <algorithm>
#include <numeric>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <queue>

#include "SysEvent.h"
#include "../FormatException.h"
#include "../objects/Time.h"
#include "../objects/Client.h"
#include "../system/SysEventExceptions.h"

class Dispatcher {
public:
    explicit Dispatcher(std::ifstream &file);

    void showAllEvents() const;

    friend std::ostream &operator<<(std::ostream &os, const Dispatcher &dispatcher);

private:
    /** Получаем целое положительное число из строки и возвращаем его в виде int. **/
    int getNumber(const std::string &number) const;

    /** Получаем объект типа Time, строка должа быть в формате XX:XX, незначащие нули обязательны. **/
    std::shared_ptr<Time> getTime(const std::string &time) const;

    /** Проверяем имя клиента, должно содержать символы из алфавита: [a-z], [0-9], _, -. **/
    bool checkClientName(const std::string &name) const noexcept;

    /** Проверяем id event, должно соответсовать одному из id в установленном перечислении. **/
    bool checkIdEvent(int id) const noexcept;

    /** Проверяем номер стола, должно значение быть в преедлах [1, N], где N - кол-во столов. **/
    bool checkTableNumber(int table) const noexcept;

    /** Получаем вектор strings аттрибутов в строке события, каждый элемент должен быть разделен через пробел */
    std::vector<std::string> getAttributesOfEventStrings(const std::string &line) const;

    /** Загрузить Event в диспетчер и получить выходну реакцию, если возможно" **/
    void feed(std::shared_ptr<SysEvent>& pEvent);

    void kickOutAllVisitors();

    std::shared_ptr<Client> getClientByEvent(std::shared_ptr<SysEvent>& pEvent);

    std::vector<int> getIdOfFreeTables();

    int numberOfTables;
    int price;
    std::shared_ptr<Time> tBegin;
    std::shared_ptr<Time> tEnd;

    std::list<std::shared_ptr<SysEvent>> events;
    std::list<std::shared_ptr<Client>> clients;
    std::map<int, std::shared_ptr<Client>> tables;
    std::list<std::shared_ptr<Client>> waitList;

    static inline const std::vector<int> ids = {SysEvent::EVENT_1_CAME,
                                                SysEvent::EVENT_2_CHANGE_SEAT,
                                                SysEvent::EVENT_3_WAIT,
                                                SysEvent::EVENT_4_GONE,
                                                SysEvent::EVENT_11_KICKED_OUT,
                                                SysEvent::EVENT_12_SIT_DOWN,
                                                SysEvent::EVENT_13_ERROR};
    // нарушить правило одного определения, линковщик заркывает глаза


};

#endif
