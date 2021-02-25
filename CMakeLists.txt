cmake_minimum_required(VERSION 3.7.0)

project(qcustomplotdemo VERSION 1.0.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(QCustomPlot)

add_executable(qcustomplotdemo
  mainwindow.cpp
  main.cpp
)

target_link_libraries(qcustomplotdemo
  Qt5::Widgets Qt5::PrintSupport ${QCustomPlot_LIBRARIES}
  )

install(TARGETS qcustomplotdemo)
