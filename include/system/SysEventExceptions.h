#ifndef MY_SYSTEM_SYSEVENTEXCEPTIONS_H
#define MY_SYSTEM_SYSEVENTEXCEPTIONS_H

class SysEventException {
public:
    static inline constexpr auto YOU_SHALL_NOT_PASS = "YouShallNotPass";
    static inline constexpr auto NOT_OPEN_YET= "NotOpenYet";
    static inline constexpr auto PLACE_IS_BUSY = "PlaceIsBusy";
    static inline constexpr auto CLIENT_UNKNOWN = "ClientUnknown";
    static inline constexpr auto I_CAN_WAIT_NO_LONGER = "ICanWaitNoLonger!";
};
#endif
