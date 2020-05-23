set(BINARY spo_Compiler)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.c *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_run ${SOURCES})

IF (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static -lstdc++ -lpthread")
ENDIF ()

add_library(${BINARY}_lib STATIC ${SOURCES} Token.hpp Node.hpp StackTraceGenerator.hpp variable.hpp variable_table.hpp semantic_analyzer.hpp)

if (Boost_FOUND)
    target_link_libraries(${BINARY}_run ${Boost_LIBRARIES})
endif ()