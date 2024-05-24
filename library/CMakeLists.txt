file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(alttest SHARED ${SRC_FILES})

target_include_directories(alttest
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(alttest PRIVATE ALTTEST_EXPORT)