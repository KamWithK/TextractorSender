cmake_minimum_required(VERSION 3.4.0)
project(TextractorSender VERSION 0.1.0)

include_directories(Extension)

include(FetchContent)

add_library(
    TextractorSender SHARED
    Extension/ExtensionImpl.cpp
    Extension/Extension.cpp
    Extension/Extension.h
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


