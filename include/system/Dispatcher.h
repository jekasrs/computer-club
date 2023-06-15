#ifndef MY_SYSTEM_DISPATCHER_H
#define MY_SYSTEM_DISPATCHER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <numeric>

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
    /** Получить целое положительное число из строки и возвращаем его в виде int. **/
    int getNumber(const std::string &number) const;

    /** Получить объект типа Time, строка должа быть в формате XX:XX, незначащие нули обязательны. **/
    std::shared_ptr<Time> getTime(const std::string &time) const;

    /** Проверить имя клиента, должно содержать символы из алфавита: [a-z], [0-9], _, -. **/
    bool checkClientName(const std::string &name) const noexcept;

    /** Проверить id event, должно соответсовать одному из id в установленном перечислении. **/
    bool checkIdEvent(int id) const noexcept;

    /** Проверить номер стола, должно значение быть в преедлах [1, N], где N - кол-во столов. **/
    bool checkTableNumber(int table) const noexcept;

    /** Получить вектор strings аттрибутов в строке события, каждый элемент должен быть разделен через пробел. */
    std::vector<std::string> getAttributesOfEventStrings(const std::string &line) const;

    /** Загрузить Event в диспетчер и получить выходну реакцию, если возможно" **/
    void feed(std::shared_ptr<SysEvent>& pEvent);

    /** Сгенерировать исходящие события для всех посетителей, которые остались в клубе к окночанию работы. " **/
    void kickOutAllVisitors();

    /** Проверить, есть клиент уже в списке клиентов **/
    bool isClientIn(const std::string& name) const noexcept;

    /** Получить клиента из спискоа клиентов, которые находятся в клубе **/
    std::shared_ptr<Client> getClientByName(const std::string& name) const;

    /** Добавить клиента в список клиентов, которые находятся в клубе **/
    std::shared_ptr<Client> addClient(const std::string& name);

    /** Удалить клиента из списка клиентов, которые находятся в клубе **/
    void removeClient(const std::shared_ptr<Client>& client);

    /** Получить id столов, которые не заняты клиентами **/
    std::vector<int> getIdOfFreeTables();

    int countSum(const Time& time);
    Time countTotalTime(const Time& begin, const Time& end);

    std::shared_ptr<SysEvent> getLastEventOfEnterByClientAndTable(const std::shared_ptr<Client>& currentClient, int table);
    void recalculateProfit(const std::shared_ptr<Client>& currentClient, const std::shared_ptr<SysEvent>& pEvent, int table);

    /** General settings **/
    int numberOfTables;
    int price;
    std::shared_ptr<Time> tBegin;
    std::shared_ptr<Time> tEnd;

    /** containers for processing **/
    std::list<std::shared_ptr<SysEvent>> events;
    std::map<int, std::shared_ptr<Client>> tables;
    std::list<std::shared_ptr<Client>> clients;
    std::list<std::shared_ptr<Client>> waitList;
    std::map<int, std::pair<int, std::shared_ptr<Time>>> tablesProfit;

    /** ids of Events **/
    static inline const std::vector<int> ids = {SysEvent::EVENT_1_CAME,
                                                SysEvent::EVENT_2_CHANGE_SEAT,
                                                SysEvent::EVENT_3_WAIT,
                                                SysEvent::EVENT_4_GONE,
                                                SysEvent::EVENT_11_KICKED_OUT,
                                                SysEvent::EVENT_12_SIT_DOWN,
                                                SysEvent::EVENT_13_ERROR};
};

#endif
