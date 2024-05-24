file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

SET(TARGET_NAME test-app)

add_executable(${TARGET_NAME} ${SRC_FILES})

find_library(alttest NAMES alttest)
target_link_libraries(${TARGET_NAME} PRIVATE alttest)

target_include_directories(${TARGET_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

