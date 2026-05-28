cmake_minimum_required(VERSION 3.10.0)
project(DSA-PQ VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.h 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/heappq.h 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/arraypq.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/packet.h)

set(SOURCE  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/heappq.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/arraypq.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/packet.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE}) 

include(CTest)
enable_testing()