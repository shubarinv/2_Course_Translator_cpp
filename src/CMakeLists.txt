set(BINARY spo_Compiler)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES} Token.hpp Node.hpp)
if (Boost_FOUND)
    target_link_libraries(${BINARY}_run ${Boost_LIBRARIES})
endif ()