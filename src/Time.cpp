#include "../include/objects/Time.h"

Time::Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

int Time::getHours() const noexcept {
    return hours;
}

int Time::getMinutes() const noexcept {
    return minutes;
}

std::ostream &operator<<(std::ostream &os, const Time &time) {
    os << std::setw(2) << std::setfill('0') << time.hours << ":" << std::setw(2) << std::setfill('0') << time.minutes;
    return os;
}

bool Time::operator>(const Time &rhs) const noexcept {
    if (hours == rhs.hours) {
        if (minutes != rhs.minutes) {
            return minutes > rhs.minutes;
        } else {
            return false;
        }
    } else {
        return hours > rhs.hours;
    }
}

bool Time::operator<(const Time &rhs) const noexcept {
    if (hours == rhs.hours) {
        if (minutes != rhs.minutes) {
            return minutes < rhs.minutes;
        } else {
            return false;
        }
    } else {
        return hours < rhs.hours;
    }
}

bool Time::operator==(const Time &rhs) const noexcept {
    return !((*this) < rhs) && !((*this) > rhs);
}

void Time::setHours(int hours) {
    Time::hours = hours;
}

void Time::setMinutes(int minutes) {
    Time::minutes = minutes;
}
