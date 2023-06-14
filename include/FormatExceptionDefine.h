#ifndef MY_SYSTEM_FORMATEXCEPTIONDEFINE_H
#define MY_SYSTEM_FORMATEXCEPTIONDEFINE_H

class FormatExceptionDefine {
public:
    static inline constexpr auto TIME_FORMAT_LENGTH_EXCEPTION = "Формат времени должен быть вида XX:XX";
    static inline constexpr auto TIME_FORMAT_H1_EXCEPTION = "Формат времени должен быть вида [X]X:XX, где [X] - цифра (0, 1, 2)";
    static inline constexpr auto TIME_FORMAT_H2_EXCEPTION = "Формат времени должен быть вида X[X]:XX, где [X] - цифра";
    static inline constexpr auto TIME_FORMAT_COLON_EXCEPTION = "Формат времени должен быть вида XX[:]XX";
    static inline constexpr auto TIME_FORMAT_M1_EXCEPTION = "Формат времени должен быть вида XX:[X]X, где [X] - цифра (0, 1,.. 5)";
    static inline constexpr auto TIME_FORMAT_M2_EXCEPTION = "Формат времени должен быть вида XX:X[X], где [X] - цифра";
    static inline constexpr auto TIME_FORMAT_H2_OUT_OF_RANGE = "Формат времени должен быть вида [XX]:XX, где [XX] - число (20, 21,.. 23)";
    static inline constexpr auto TIME_FORMAT_SPACE_EXCEPTION = "Временной промежуток должен содержать только один пробел";
    static inline constexpr auto NUMBER_FORMAT_EXCEPTION = " не является цифрой. Этот символ(ы) не могут находиться в числе";
    static inline constexpr auto NAME_FORMAT_EXCEPTION = "Имя клиента должно содержать только [a-z], [0-9], -, _ ";
    static inline constexpr auto ATTR_FORMAT_SPACE_EXCEPTION = "Количество атрибутов в событии должно быть от 3 до 4. Найдено: ";
    static inline constexpr auto ATTR_FROMAT_NUMBER_EXCEPTION = "Для события 'клиент сел за стол' (2) должно быть установлено 4 атрибута. Найдено: ";

    static inline constexpr auto INVALID_TIME_RANGE = "Временной диапазон не верен";
    static inline constexpr auto INVALID_VALUE_ZERO = "Число должно быть положительным";
    static inline constexpr auto INVALID_TIME_ORDER = "Время событий должно быть последовательным и линейным";

    static inline constexpr auto NOT_FOUND_ID_EVENT = " не найден в допустимых идентификаторах событий";
    static inline constexpr auto NOT_FOUND_TABLE_EVENT = " не найден в допустимых идентификаторах столов";

    static inline constexpr auto OUT_OF_RANGE_TIME_EXCEPTION = "Время события вне времени работы клуба";
};

#endif
