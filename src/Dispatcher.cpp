#include "../include/system/Dispatcher.h"

Dispatcher::Dispatcher(std::ifstream& file) {

    if (!file.is_open()) {
        file.close();
        std::cout << "Unable to open file" << std::endl;
    }

    std::string line;
    int counterLine = 0;

    try {
        /** чтение, проверка и присвоение основных значений системы [numberOfTables, tBegin, tEnd, price]: **/

        // 1. numberOfTables :
                    std::getline(file, line);
            counterLine++;
            numberOfTables = getNumber(line);

        // 2. tBegin, tEnd :
        std::getline(file, line);
        counterLine++;
        std::vector<std::string> words = std::vector<std::string>();
        std::istringstream iss(line);
        std::string word;

        while (std::getline(iss, word, ' '))
            words.push_back(word);

        if (words.size() > 2)
            throw FormatException(FormatExceptionDefine::TIME_SPACE_EXCEPTION);

        tBegin = getTime(words[0]);
        tEnd = getTime(words[1]);

        if (!(*tEnd > *tBegin))
            throw FormatException(FormatExceptionDefine::INVALID_TIME_RANGE);

        // 3. price :
                std::getline(file, line);
        counterLine++;
        price = getNumber(line);

        for(int i = 1; i <= numberOfTables; ++i)
            tables.insert(std::make_pair(i, std::shared_ptr<Client>()));

        events = std::list<std::shared_ptr<SysEvent>>();
        clients = std::list<std::shared_ptr<Client>>();

        /** чтение, проверка и запись событий в список событий системы **/

        // Обрабатываем первое событие, чтобы проинизицализировать
        // lastTime - указатель на время последнего-прочтанного события
        std::getline(file, line);
        counterLine++;
        std::vector<std::string> attributes = getAttributesOfEventStrings(line);
        std::shared_ptr<SysEvent> sysEvent;

        auto time = getTime(attributes[0]);
        Time *lastTime = time.get();
        int id = getNumber(attributes[1]);
        if (!checkIdEvent(id))
            throw FormatException(attributes[1] + FormatExceptionDefine::NOT_FOUND_ID_EVENT);
        std::string client = attributes[2];
        if (!checkClientName(attributes[2]))
            throw FormatException(attributes[2] + FormatExceptionDefine::INVALID_CLIENT_NAME);
        if (id == SysEvent::EVENT_2_CHANGE_SEAT) {
            int table = getNumber(attributes[3]);
            if (!checkTableNumber(table))
                throw FormatException(attributes[3] + FormatExceptionDefine::NOT_FOUND_TABLE_EVENT);

            sysEvent = std::make_shared<SysEvent>(SysEvent(id, time, client, table));
        } else {
            sysEvent = std::make_shared<SysEvent>(SysEvent(id, time, client));
        }
        this->feed(sysEvent);
        while (std::getline(file, line)) {
            counterLine++;
            attributes = getAttributesOfEventStrings(line);
            time = getTime(attributes[0]);
            if (*lastTime > *time)
                throw FormatException(FormatExceptionDefine::TIMELINE_INVALID_ORDER);

            if ((*time > *tEnd) || (*time < *tBegin))
                throw FormatException(FormatExceptionDefine::TIMELINE_INVALID_ORDER);

            lastTime = time.get();

            id = getNumber(attributes[1]);
            if (!checkIdEvent(id))
                throw FormatException(attributes[1] + FormatExceptionDefine::NOT_FOUND_ID_EVENT);

            client = attributes[2];
            if (!checkClientName(attributes[2]))
                throw FormatException(attributes[2] + FormatExceptionDefine::INVALID_CLIENT_NAME);

            if (id != SysEvent::EVENT_2_CHANGE_SEAT) {
                sysEvent = std::make_shared<SysEvent>(SysEvent(id, time, client));
            } else {
                int table = getNumber(attributes[3]);
                if (!checkTableNumber(table))
                    throw FormatException(attributes[3] + FormatExceptionDefine::NOT_FOUND_TABLE_EVENT);

                sysEvent = std::make_shared<SysEvent>(SysEvent(id, time, client, table));
            }
            this->feed(sysEvent);
        }

        // Убираем всех клиентов из клуба, кототые не ушли во время до закрытия
        kickOutVisitors();

    }
    catch (FormatException &ex) {
        throw FormatException(counterLine, line, ex.getInfo());
    }
}

int Dispatcher::getNumber(const std::string& number) const {
    try {
        for (char c: number) {
            if (!isdigit(c))
                throw FormatException(
                        std::string("\'" + std::string(1, c) + "\'" + FormatExceptionDefine::NOT_DIGIT_CHAR_EXCEPTION));
        }
        int N = std::stoi(number);
        return N;
    }
    catch (std::invalid_argument const &ex) {
        throw FormatException(number + FormatExceptionDefine::NOT_DIGIT_CHAR_EXCEPTION);
    }
}

bool Dispatcher::checkClientName(const std::string& name) const noexcept {
    if (name.empty())
        return false;
    for (char c: name) {
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_') || (c == '-')) continue;
        else
            return false;
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
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_H1_OUT_OF_RANGE);

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
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_M1_OUT_OF_RANGE);

    char m2 = time[4];
    if (!isdigit(m2))
        throw FormatException(FormatExceptionDefine::TIME_FORMAT_M2_EXCEPTION);

    std::string hours = std::string(1, h1) + h2;
    std::string minutes = std::string(1, m1) + m2;

    int h = std::stoi(hours);
    int m = std::stoi(minutes);

    std::shared_ptr<Time> t = std::make_shared<Time>(Time(h,m));
    return t;
}

std::vector<std::string> Dispatcher::getAttributesOfEventStrings(const std::string& line) const {
    std::vector<std::string> attrs = std::vector<std::string>();
    std::istringstream iss(line);
    std::string attr;

    while (std::getline(iss, attr, ' '))
        attrs.push_back(attr);

    if (attrs.size() < 3 || attrs.size() > 4)
        throw FormatException(FormatExceptionDefine::ATTR_SPACE_EXCEPTION + std::to_string(attrs.size()));

    if (attrs[1] == std::to_string(SysEvent::EVENT_2_CHANGE_SEAT) && attrs.size() != 4)
        throw FormatException(FormatExceptionDefine::ATTR_NUMBER_EXCEPTION + std::to_string(attrs.size()));

    return attrs;
}

void Dispatcher::showAllEvents() const {
    std::cout << *tBegin << std::endl;
    for (auto i : events) {
        std::cout << *i;
    }
    std::cout << *tEnd << std::endl;
}

void Dispatcher::feed(std::shared_ptr<SysEvent> pEvent) {
    events.push_back(pEvent);

    auto it = std::find_if(clients.begin(), clients.end(), [&pEvent](std::shared_ptr<Client> element ){
        return pEvent->getClient() == element->getName();
    });

    std::shared_ptr<Client> currentClient;
    if (it == clients.end()) {
        currentClient = std::make_shared<Client>(Client(pEvent->getClient()));
        clients.push_back(currentClient);
    }
    else {
        currentClient = *it;
    }

    if (pEvent->getId() == SysEvent::EVENT_1_CAME) {

        // пришел в не рабочие часы
        if (*(pEvent->getTime()) < *tBegin || *(pEvent->getTime()) > *tEnd) {
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(SysEvent(SysEvent::EVENT_13_ERROR, pEvent->getTime(),SysEventException::NOT_OPEN_YET, true));
            events.push_back(error);
            return;
        }

        // уже есть в клубе
        if (currentClient->isHere()){
            pEvent->setSpoiled(true);
            auto error = std::make_shared<SysEvent>(SysEvent(SysEvent::EVENT_13_ERROR, pEvent->getTime(),SysEventException::YOU_SHALL_NOT_PASS, true));
            events.push_back(error);
            return;
        }

        currentClient->setIsHere(true);
    }

    else if (pEvent->getId() == SysEvent::EVENT_2_CHANGE_SEAT) {

    }
    else if (pEvent->getId() == SysEvent::EVENT_3_WAIT) {

    }
    else if (pEvent->getId() == SysEvent::EVENT_4_GONE) {

    }
}

std::ostream &operator<<(std::ostream &os, const Dispatcher &dispatcher) {
    os << "numberOfTables: " << dispatcher.numberOfTables << "\n" << "price: " << dispatcher.price << "\n"
       << "tBegin: "
       << *dispatcher.tBegin << "\n" << "tEnd: " << *dispatcher.tEnd << std::endl;
    return os;
}

void Dispatcher::kickOutVisitors() {}

