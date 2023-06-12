#include "../include/objects/Time.h"

Time::Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

int Time::getHours() const {
    return hours;
}

int Time::getMinutes() const {
    return minutes;
}

std::ostream &operator<<(std::ostream &os, const Time &time) {
    os << std::setw(2) << std::setfill('0') << time.hours << ":" << std::setw(2) << std::setfill('0') << time.minutes;
    return os;
}

bool Time::operator>(const Time &rhs) const {
    if (hours == rhs.hours) {
        if (minutes == rhs.minutes)
            return false;
        else
            return minutes > rhs.minutes;
    }
    else
        return hours > rhs.hours;
}

bool Time::operator<(const Time &rhs) const {
    if (hours == rhs.hours) {
        if (minutes == rhs.minutes)
            return false;
        else
            return minutes < rhs.minutes;
    }
    else
        return hours < rhs.hours;
}

bool Time::operator==(const Time &rhs) const {
    return !((*this) < rhs) && !((*this) > rhs);
}
