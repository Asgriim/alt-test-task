file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

set(LIB_NAME alttest)

add_library(${LIB_NAME} SHARED ${SRC_FILES})

target_include_directories(${LIB_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(jsoncpp REQUIRED)
find_package(CURL REQUIRED)

target_link_libraries(${LIB_NAME} PRIVATE jsoncpp_lib ${CURL_LIBRARIES})

target_compile_definitions(${LIB_NAME} PRIVATE ${LIB_NAME}_EXPORT)