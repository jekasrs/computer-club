#ifndef MY_SYSTEM_TIME_H
#define MY_SYSTEM_TIME_H

#include <ostream>
#include <iomanip>

class Time {
public:
    Time(int hours, int minutes);
    int getHours() const;
    int getMinutes() const;

    friend std::ostream &operator<<(std::ostream &os, const Time &time);

    bool operator>(const Time &rhs) const;
    bool operator<(const Time &rhs) const;
    bool operator==(const Time &rhs) const;

private:
    const int hours;
    const int minutes;
};

#endif
