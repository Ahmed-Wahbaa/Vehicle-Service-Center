cmake_minimum_required(VERSION 3.16)
project(VehicleServiceCenterGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 1. Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 2. Find PostgreSQL (Automatically finds the correct paths on Ubuntu)
find_package(PostgreSQL REQUIRED)

# 3. Define the executable
add_executable(VehicleServiceCenterGUI
    main.cpp
    mainwindow.cpp
    mainwindow.h
    vehicle_service_center.hpp
)

# 4. Tell the compiler where the PostgreSQL headers (libpq-fe.h) are
target_include_directories(VehicleServiceCenterGUI PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# 5. Link the libraries
target_link_libraries(VehicleServiceCenterGUI PRIVATE
    Qt6::Widgets
    ${PostgreSQL_LIBRARIES}
)
