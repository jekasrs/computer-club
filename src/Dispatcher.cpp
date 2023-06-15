#include "../include/system/Dispatcher.h"

Dispatcher::Dispatcher(std::ifstream& file) {
    std::string line;
    int counterLine = 0;

    try {
        /** чтение, проверка и присвоение основных значений системы [numberOfTables, tBegin, tEnd, price]: **/

        // 1. numberOfTables :
        std::getline(file, line);
        counterLine++;
        int num = getNumber(line);
        if (num > 0) {
            numberOfTables = num;
        }
        else {
            throw FormatException(FormatExceptionDefine::INVALID_VALUE_ZERO);
        }

        // 2. tBegin, tEnd :
        std::getline(file, line);
        counterLine++;
        std::vector<std::string> words = std::vector<std::string>();
        std::istringstream iss(line);
        std::string word;

        while (std::getline(iss, word, ' ')) {
            words.push_back(word);
        }

        if (words.size() > 2) {
            throw FormatException(FormatExceptionDefine::TIME_FORMAT_SPACE_EXCEPTION);
        }

        tBegin = getTime(words[0]);
        tEnd = getTime(words[1]);

        if ((*tEnd == *tBegin)) {
            throw FormatException(FormatExceptionDefine::INVALID_TIME_RANGE);
        }

        // 3. price :
        std::getline(file, line);
        counterLine++;
        int money = getNumber(line);
        if (money > 0){
            price = money;
        }
        else{
            throw FormatException(FormatExceptionDefine::INVALID_VALUE_ZERO);
        }

        for(int i = 1; i <= numberOfTables; ++i) {
            tables.insert(std::make_pair(i, std::shared_ptr<Client>()));
            tablesProfit.insert(std::make_pair(i, std::make_pair(0, std::make_shared<Time>(0, 0))));
        }

        events = std::list<std::shared_ptr<SysEvent>>();
        clients = std::list<std::shared_ptr<Client>>();
        waitList = std::list<std::shared_ptr<Client>>();

        /** чтение, проверка и запись событий в список событий системы **/

        // Обрабатываем первое событие, чтобы проинизицализировать
        // lastTime - указатель на время последнего-прочтанного события
        std::getline(file, line);
        counterLine++;
        std::vector<std::string> attributes = getAttributesOfEventStrings(line);
        std::shared_ptr<SysEvent> sysEvent;

        auto time = getTime(attributes[0]);
        if ((*time > *tEnd) && (*time < *tBegin)) {
            throw FormatException(FormatExceptionDefine::OUT_OF_RANGE_TIME_EXCEPTION);
        }
        Time *lastTime = time.get();
        int id = getNumber(attributes[1]);
        if (!checkIdEvent(id)) {
            throw FormatException(attributes[1] + FormatExceptionDefine::NOT_FOUND_ID_EVENT);
        }
        std::string client = attributes[2];
        if (!checkClientName(attributes[2])) {
            throw FormatException(attributes[2] + FormatExceptionDefine::NAME_FORMAT_EXCEPTION);
        }
        if (id == SysEvent::EVENT_2_CHANGE_SEAT) {
            int table = getNumber(attributes[3]);
            if (!checkTableNumber(table)) {
                throw FormatException(attributes[3] + FormatExceptionDefine::NOT_FOUND_TABLE_EVENT);
            }
            sysEvent = std::make_shared<SysEvent>(id, time, client, table);
        }
        else {
            sysEvent = std::make_shared<SysEvent>(id, time, client);
        }
        this->feed(sysEvent);
        while (!file.eof()) {
            std::getline(file, line);
            counterLine++;
            attributes = getAttributesOfEventStrings(line);
            time = getTime(attributes[0]);

            if (*lastTime > *time) {
                throw FormatException(FormatExceptionDefine::INVALID_TIME_ORDER);
            }

            if ((*time > *tEnd) && (*time < *tBegin)) {
                throw FormatException(FormatExceptionDefine::OUT_OF_RANGE_TIME_EXCEPTION);
            }
            lastTime = time.get();

            id = getNumber(attributes[1]);
            if (!checkIdEvent(id)) {
                throw FormatException(attributes[1] + FormatExceptionDefine::NOT_FOUND_ID_EVENT);
            }
            client = attributes[2];
            if (!checkClientName(attributes[2])) {
                throw FormatException(attributes[2] + FormatExceptionDefine::NAME_FORMAT_EXCEPTION);
            }
            if (id != SysEvent::EVENT_2_CHANGE_SEAT) {
                sysEvent = std::make_shared<SysEvent>(id, time, client);
            } else {
                int table = getNumber(attributes[3]);
                if (!checkTableNumber(table)) {
                    throw FormatException(attributes[3] + FormatExceptionDefine::NOT_FOUND_TABLE_EVENT);
                }
                sysEvent = std::make_shared<SysEvent>(id, time, client, table);
            }
            this->feed(sysEvent);
        }

        // Убираем всех клиентов из клуба, кототые не ушли во время (до закрытия)
        kickOutAllVisitors();
    }
    catch (const FormatException &ex) {
        throw FormatException(counterLine, line, ex.getInfo());
    }
}

int Dispatcher::getNumber(const std::string& number) const {
    try {
        for (char c: number) {
            if (!isdigit(c)) {
                throw FormatException(
                        std::string("\'" + std::string(1, c) + "\'" + FormatExceptionDefine::NUMBER_FORMAT_EXCEPTION));
            }
        }
        int N = std::stoi(number);
        return N;
    }
    catch (const std::invalid_argument &ex) {
        throw FormatException(number + FormatExceptionDefine::NUMBER_FORMAT_EXCEPTION);
    }
}

bool Dispatcher::checkClientName(const std::string& name) const noexcept {
    if (name.empty()){
        return false;
    }
    for (char c: name) {
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_') || (c == '-')) {
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

bool Dispatcher::checkIdEvent(int id) const noexcept {
    auto it = std::find(Dispatcher::ids.begin(), Dispatcher::ids.end(), id);
    if (it == Dispatcher::ids.end())
        return false;
    return true;
}

bool Dispatcher::checkTableNumber(int table) const noexcept {
    bool isFound = false;
    for (auto it = tables.begin(); it != tables.end(); ++it) {
        int key = it->first;
        if (key == table) {
            isFound = true;
            return isFound;
        }
    }
    return isFound;
}

std::shared_ptr<Time> Dispatcher::getTime(const std::string& time) const {
    if (time.length() != 5)
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_LENGTH_EXCEPTION);

    char h1 = time[0];
    if (!isdigit(h1))
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_H1_EXCEPTION);

    if (h1 < '0' || h1 > '2')
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_H1_EXCEPTION);

    char h2 = time[1];
    if (!isdigit(h2))
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_H2_EXCEPTION);

    if (h1 == '2' && h2 > '3')
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_H2_OUT_OF_RANGE);

    char colon = time[2];
    if (colon != ':')
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_COLON_EXCEPTION);

    char m1 = time[3];
    if (!isdigit(m1))
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_M1_EXCEPTION);

    if (m1 < '0' || m1 > '5')
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_M1_EXCEPTION);

    char m2 = time[4];
    if (!isdigit(m2))
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_M2_EXCEPTION);

    std::string hours = std::string(1, h1) + h2;
    std::string minutes = std::string(1, m1) + m2;

    int h = std::stoi(hours);
    int m = std::stoi(minutes);

    std::shared_ptr<Time> t = std::make_shared<Time>(h,m);
    return t;
}

std::vector<std::string> Dispatcher::getAttributesOfEventStrings(const std::string& line) const {
    std::vector<std::string> attrs = std::vector<std::string>();
    std::istringstream iss(line);
    std::string attr;

    while (std::getline(iss, attr, ' '))
        attrs.push_back(attr);

    if (attrs.size() < 3 || attrs.size() > 4)
        throw FormatException(FormatExceptionDefine::ATTR_FORMAT_SPACE_EXCEPTION + std::to_string(attrs.size()));

    if (attrs[1] == std::to_string(SysEvent::EVENT_2_CHANGE_SEAT) && attrs.size() != 4)
        throw FormatException(FormatExceptionDefine::ATTR_FROMAT_NUMBER_EXCEPTION + std::to_string(attrs.size()));

    return attrs;
}

void Dispatcher::showAllEvents() const {
    std::cout << *tBegin << std::endl;
    for (auto i : events) {
        std::cout << *i;
    }
    std::cout << *tEnd << std::endl;
    for (auto i: tablesProfit){
        std::cout << i.first << " " << i.second.first << " " << *i.second.second << std::endl;
    }
}

void Dispatcher::feed(std::shared_ptr<SysEvent>& pEvent) {
    events.push_back(pEvent);

    std::shared_ptr<Client> currentClient;
    if (isClientIn(pEvent->getClientName()))
        currentClient = getClientByName(pEvent->getClientName());
    else
        currentClient = addClient(pEvent->getClientName());

    if (pEvent->getId() == SysEvent::EVENT_1_CAME) {
        // пришел в нерабочие часы
        // if (*(pEvent->getTime()) < *tBegin || *(pEvent->getTime()) > *tEnd) {
        if ((*pEvent->getTime() > *tEnd) && (*pEvent->getTime() < *tBegin)) {
                pEvent->setSpoiled(true);
                auto error = std::make_shared<SysEvent>(
                        SysEvent::EVENT_13_ERROR, pEvent->getTime(), SysEventException::NOT_OPEN_YET, true);
                events.push_back(error);
                removeClient(currentClient);
                return;
            }

        // уже есть в клубе
        if (currentClient->isHere()) {
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(
                    SysEvent::EVENT_13_ERROR, pEvent->getTime(), SysEventException::YOU_SHALL_NOT_PASS, true);
            events.push_back(error);
            return;
        }

        // запускаме клиента в клуб
        currentClient->setIsHere(true);
    }
    else if (pEvent->getId() == SysEvent::EVENT_2_CHANGE_SEAT) {

        // стол занят
        if (tables[pEvent.get()->getTable()] != nullptr) {
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(
                    SysEvent::EVENT_13_ERROR, pEvent->getTime(), SysEventException::PLACE_IS_BUSY, true);
            events.push_back(error);
            return;
        }

        // клиента нет в клубе
        if (!currentClient->isHere()) {
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(
                    SysEvent::EVENT_13_ERROR, pEvent->getTime(), SysEventException::CLIENT_UNKNOWN, true);
            events.push_back(error);
            return;
        }

        // убираем клиента из мапы за столом, которым он сидел
        int foundTable;
        bool hasFound = false;
        for (const auto &pair: tables) {
            if (pair.second == currentClient) {
                foundTable = pair.first;
                hasFound = true;
            }
        }

        if (hasFound) {
            recalculateProfit(currentClient, pEvent, foundTable);
            tables[foundTable] = nullptr;
        }

        // пересаживаем за новый стол
        tables[pEvent.get()->getTable()] = currentClient;

    }
    else if (pEvent->getId() == SysEvent::EVENT_3_WAIT) {
        // клиента нет в клубе
        if (!currentClient->isHere()) {
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(
                    SysEvent::EVENT_13_ERROR, pEvent->getTime(), SysEventException::CLIENT_UNKNOWN, true);
            events.push_back(error);
            return;
        }

        // есть свободное место
        auto foundFreeTables = getIdOfFreeTables();
        if (foundFreeTables.size() > 0) {
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(
                    SysEvent::EVENT_13_ERROR, pEvent->getTime(), SysEventException::I_CAN_WAIT_NO_LONGER, true);
            events.push_back(error);
            return;
        }

        // в очереди ожидания клиентов [с учетом current клиента] больше(>), чем общее число столов
        if (waitList.size() == numberOfTables) {
            // генерируем исходящее событие: EVENT_11_KICKED_OUT
            pEvent->setSpoiled(true);
            auto outEvent = std::make_shared<SysEvent>(
                    SysEvent::EVENT_11_KICKED_OUT, pEvent->getTime(), currentClient->getName());
            events.push_back(outEvent);
            removeClient(currentClient);
            return;
        }

        // ставим в очередь
        waitList.push_back(currentClient);
    }
    else if (pEvent->getId() == SysEvent::EVENT_4_GONE) {
        // клиента нет в клубе
        if (!currentClient->isHere()) {
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(
                    SysEvent::EVENT_13_ERROR, pEvent->getTime(), SysEventException::CLIENT_UNKNOWN, true);
            events.push_back(error);
            return;
        }

        // находим стол где сидит current клиент, который хочет уйти
        int foundTable;
        for (const auto &pair: tables) {
            if (pair.second == currentClient) {
                foundTable = pair.first;
            }
        }

        recalculateProfit(currentClient, pEvent, foundTable);

        // удаляем current клиента из стола
        removeClient(currentClient);
        tables[foundTable] = nullptr;

        // сажаем туда первого из очереди и генерируем исходящее событие
        if (!waitList.empty()) {
            auto client = waitList.front();

            auto outEvent = std::make_shared<SysEvent>(SysEvent::EVENT_12_SIT_DOWN, pEvent->getTime(), client->getName(), foundTable);

            tables[foundTable] = client;
            waitList.remove(client);
            events.push_back(outEvent);
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Dispatcher &dispatcher) {
    os << "numberOfTables: " << dispatcher.numberOfTables << "\n" << "price: " << dispatcher.price << "\n"
       << "tBegin: "
       << *dispatcher.tBegin << "\n" << "tEnd: " << *dispatcher.tEnd << std::endl;
    return os;
}

void Dispatcher::kickOutAllVisitors() {
    // Сортировка списка clients по полю name
    clients.sort(Client::compareByName);

    for (auto client : clients) {
        if (client->isHere()) {
            auto outEvent = std::make_shared<SysEvent>(
                    SysEvent::EVENT_11_KICKED_OUT, tEnd, client->getName());
            events.push_back(outEvent);
            int foundTable;
            bool hasFound = false;
            for (const auto &pair: tables) {
                if (pair.second == client) {
                    foundTable = pair.first;
                    hasFound = true;
                }
            }
            if (hasFound) recalculateProfit(client, outEvent, foundTable);
        }
    }
}

std::vector<int> Dispatcher::getIdOfFreeTables() {
    std::vector<int> foundTables = std::vector<int>();
    for (const auto &pair: tables) {
        if (pair.second == nullptr) {
            foundTables.push_back(pair.first);
        }
    }

    return foundTables;
}

bool Dispatcher::isClientIn(const std::string& name) const noexcept {
    auto it = std::find_if(clients.begin(), clients.end(), [&name](std::shared_ptr<Client> element) {
        return name == element->getName();
    });

    return it != clients.end();
}

std::shared_ptr<Client> Dispatcher::getClientByName(const std::string& name) const {
    auto it = std::find_if(clients.begin(), clients.end(), [&name](std::shared_ptr<Client> element) {
        return name == element->getName();
    });

    return *it;
}

std::shared_ptr<Client> Dispatcher::addClient(const std::string& name) {
    std::shared_ptr<Client> currentClient = std::make_shared<Client>(name);
    clients.push_back(currentClient);

    return currentClient;
}

void Dispatcher::removeClient(const std::shared_ptr<Client>& client) {
    clients.remove(client);
}

int Dispatcher::countSum(const Time& time) {
    int coef = time.getHours();
    if (time.getMinutes() != 0) {
        coef += 1;
    }
    return coef * price;
}

Time Dispatcher::countTotalTime(const Time& begin, const Time& end) {
    int hours;
    int minutes;

    if (begin < end || begin == end) {
        hours = end.getHours() - begin.getHours();
        minutes = end.getMinutes() - begin.getMinutes();

        if (minutes < 0) {
            hours -= 1;
            minutes = 60 + minutes;
        }
    } else {
        hours = 24 - begin.getHours() + end.getHours();
        minutes = end.getMinutes() - begin.getMinutes();
        if (minutes < 0) {
            hours -= 1;
            minutes = 60 + minutes;
        }
    }

    return Time(hours, minutes);
}

std::shared_ptr<SysEvent> Dispatcher::getLastEventOfEnterByClientAndTable(const std::shared_ptr<Client>& currentClient, int table) {
    auto last_event_it = std::find_if(events.rbegin(), events.rend(), [&table](const std::shared_ptr<SysEvent> &e) {
        bool isFound = (e->getId() == SysEvent::EVENT_4_GONE) || (e->getId() == SysEvent::EVENT_11_KICKED_OUT) ||
                       (e->getId() == SysEvent::EVENT_2_CHANGE_SEAT) || (e->getId() == SysEvent::EVENT_12_SIT_DOWN);
        isFound = isFound && (e->getTable() == table);
        isFound = isFound && !(e->isSpoiled());
        return isFound;
    });

    if (last_event_it != events.rend()) {
        return *(last_event_it++);
    } else
        return nullptr;
}

void Dispatcher::recalculateProfit(const std::shared_ptr<Client>& currentClient, const std::shared_ptr<SysEvent>& pEvent, int table) {
    // Когда клиент уходит, находим самый последний евент посадки (правый в списке),
    // и счиатем от него до нынешнего моменты показатели
    auto eventEnter = getLastEventOfEnterByClientAndTable(currentClient, table);
    if (eventEnter == nullptr) return;

    // Получаем время в условных единиц, сколько прошло от и до
    Time t = countTotalTime(*eventEnter->getTime(), *pEvent->getTime());

    // Сколько денег стол заработал
    tablesProfit[table].first += countSum(t);

    // Сколько времени стол был занят
    int hours = tablesProfit[table].second->getHours() + t.getHours();
    int minutes = tablesProfit[table].second->getMinutes() + t.getMinutes();
    if (minutes >= 60) {
        minutes -= 60;
        hours += 1;
    }
    tablesProfit[table].second->setHours(hours);
    tablesProfit[table].second->setMinutes(minutes);
}
