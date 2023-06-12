cmake_minimum_required(VERSION 3.20)
project(my_system)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_system main.cpp
        src/Time.cpp
        include/objects/Time.h src/Dispatcher.cpp
        include/system/Dispatcher.h
        include/FormatException.h src/FormatException.cpp src/SysEvent.cpp include/system/SysEvent.h src/Client.cpp include/objects/Client.h include/FormatExceptionDefine.h include/system/SysEventExceptions.h)
