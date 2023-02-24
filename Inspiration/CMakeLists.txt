cmake_minimum_required(VERSION 3.9)
project(Inspiration)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Core Gui Widgets)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)


add_executable(qlistview_all_columns_selected
  qlistview_all_columns_selected.cpp
)
target_link_libraries(qlistview_all_columns_selected Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui  Qt${QT_VERSION_MAJOR}::Widgets)
