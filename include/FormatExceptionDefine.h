#ifndef MY_SYSTEM_FORMATEXCEPTIONDEFINE_H
#define MY_SYSTEM_FORMATEXCEPTIONDEFINE_H

class FormatExceptionDefine {
public:
    static inline constexpr auto TIME_FORMAT_LENGTH_EXCEPTION = "Time must be in format XX:XX";
    static inline constexpr auto TIME_FORMAT_H1_EXCEPTION = "Time must be in format [X]X:XX, where [X] - digit number";
    static inline constexpr auto TIME_FORMAT_H2_EXCEPTION = "Time must be in format X[X]:XX, where [X] - digit number";
    static inline constexpr auto TIME_FORMAT_COLON_EXCEPTION = "Time must be in format XX[:]XX";
    static inline constexpr auto TIME_FORMAT_M1_EXCEPTION = "Time must be in format XX:[X]X, where [X] - digit number";
    static inline constexpr auto TIME_FORMAT_M2_EXCEPTION = "Time must be in format XX:X[X], where [X] - digit number";
    static inline constexpr auto TIME_FORMAT_H1_OUT_OF_RANGE = "Time must be in format [X]X:XX, where [X] can be 0, 1, 2";
    static inline constexpr auto TIME_FORMAT_M1_OUT_OF_RANGE = "Time must be in format XX:[X]X, where [X] can be 0, 1,.. 5";
    static inline constexpr auto TIME_FORMAT_H2_OUT_OF_RANGE = "Time must be in format XX:[XX], where [XX] - number 20, 21,.. 23";
    static inline constexpr auto TIME_SPACE_EXCEPTION = "One space only available in the time span";
    static inline constexpr auto NOT_DIGIT_CHAR_EXCEPTION = " is not a digit. This symbol(s) cannot be in number";
    static inline constexpr auto INVALID_TIME_RANGE = "Time range is inside out";
    static inline constexpr auto NOT_FOUND_ID_EVENT = " not found in allowed identifiers' events";
    static inline constexpr auto NOT_FOUND_TABLE_EVENT = " not found in allowed tables' events";
    static inline constexpr auto ATTR_SPACE_EXCEPTION = "Кол-во атрибутов в событии должно быть от 3 до 4 шт. Найдено: ";
    static inline constexpr auto ATTR_NUMBER_EXCEPTION = "Для события 'клиент сел за стол' (2) должно быть установлено 4 атрибута. Найдено: ";
    static inline constexpr auto TIMELINE_INVALID_ORDER = "The timing of the events must be consistent and linear";
    static inline constexpr auto INVALID_CLIENT_NAME = "Client name must contain only [a-z], [0-9], -, _ ";

};

#endif //MY_SYSTEM_FORMATEXCEPTIONDEFINE_H
