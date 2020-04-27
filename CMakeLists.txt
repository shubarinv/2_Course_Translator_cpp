cmake_minimum_required(VERSION 3.16)
project(spo_Compiler)

message(STATUS "start running cmake...")

set(Boost_INCLUDE_DIR c:/MinGWBoost/include/)
set(Boost_LIBRARY_DIR c:/MinGWBoost/lib/)
find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")

set(CMAKE_CXX_STANDARD 20)

add_executable(spo_Compiler main.cpp exceptions.cpp)

if (Boost_FOUND)
    target_link_libraries(spo_Compiler ${Boost_LIBRARIES})
endif ()