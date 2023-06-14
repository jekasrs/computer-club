cmake_minimum_required(VERSION 3.20)
project(my_system)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_system
        main.cpp
        include/objects/Time.h
        include/objects/Client.h
        include/FormatExceptionDefine.h
        include/FormatException.h
        include/system/SysEventExceptions.h
        include/system/SysEvent.h
        include/system/Dispatcher.h
        src/Time.cpp
        src/Client.cpp
        src/FormatException.cpp
        src/SysEvent.cpp
        src/Dispatcher.cpp)
