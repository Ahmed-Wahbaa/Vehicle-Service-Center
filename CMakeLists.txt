cmake_minimum_required(VERSION 3.16)
project(VehicleServiceCenterGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ==============================================================================
# EXACT POSTGRESQL PATH ON YOUR DESKTOP
# ==============================================================================
set(PG_PATH "C:/Users/ahmme/OneDrive/Desktop/SQL")

include_directories("${PG_PATH}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
link_directories("${PG_PATH}/lib")

add_executable(VehicleServiceCenterGUI
    main.cpp
    mainwindow.cpp
    mainwindow.h
    vehicle_service_center.hpp
)

target_link_libraries(VehicleServiceCenterGUI PRIVATE
    Qt6::Widgets
    "${PG_PATH}/lib/libpq.lib"
)