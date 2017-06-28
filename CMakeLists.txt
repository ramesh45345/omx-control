cmake_minimum_required(VERSION 2.8.11)

project(omx-control)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

# Specify or automatically locate sources.
# set(SRC main.cpp mainwindow.cpp mainwindow.h)
file(GLOB SRC "./*.cpp" "./*.h")

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Generate code from ui files
qt5_wrap_ui(UI_HEADERS mainwindow.ui)

# Generate rules for building source files from the resources
# qt5_add_resources(QRCS resources.qrc)

# Tell CMake to create the testapp executable
add_executable(omx-control ${SRC} ${UI_HEADERS})

# Use the Widgets module from Qt 5.
target_link_libraries(omx-control Qt5::Core Qt5::Widgets)
