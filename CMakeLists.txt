cmake_minimum_required(VERSION 3.21)
project(DynamicAssist)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/main.cpp
)

if(EXISTS "$ENV{GEODE_SDK}/cmake/GeodeSDK.cmake")
    include("$ENV{GEODE_SDK}/cmake/GeodeSDK.cmake")
elseif(EXISTS "$ENV{GEODE_SDK}/cmake/Geode.cmake")
    include("$ENV{GEODE_SDK}/cmake/Geode.cmake")
endif()