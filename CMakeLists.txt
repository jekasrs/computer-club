cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

set (PROJECT my_system) 
project (${PROJECT})


set (HEADERS 
	include/objects/Time.h
        include/objects/Client.h
        include/FormatExceptionDefine.h
        include/FormatException.h
        include/system/SysEventExceptions.h
        include/system/SysEvent.h
        include/system/Dispatcher.h)

set (SOURCES 
        main.cpp
        src/Time.cpp
        src/Client.cpp
        src/FormatException.cpp
        src/SysEvent.cpp
        src/Dispatcher.cpp)

add_executable (${PROJECT} ${HEADERS} ${SOURCES})