#ifndef MY_SYSTEM_TIME_H
#define MY_SYSTEM_TIME_H

#include <ostream>
#include <iomanip>

class Time {
public:
    Time(int hours, int minutes);
    int getHours() const noexcept;
    int getMinutes() const noexcept;

    friend std::ostream &operator<<(std::ostream &os, const Time &time);

    bool operator>(const Time &rhs) const noexcept;
    bool operator<(const Time &rhs) const noexcept;
    bool operator==(const Time &rhs) const noexcept;

private:
    int hours;
    int minutes;
};

#endif
