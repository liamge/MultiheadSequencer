cmake_minimum_required(VERSION 3.10)
project(MyMultiheadSequencer)
set(CMAKE_CXX_STANDARD 11)

if(NOT DEFINED Rack_DIR)
    set(Rack_DIR ~/Documents/Rack-SDK) # Adjust as needed
endif()

find_package(Rack REQUIRED)

add_subdirectory(res)
add_library(plugin SHARED 
    plugin.cpp
    src/MultiheadSequencer.cpp
    src/MyMultiheadSequencerWidget.cpp
)
target_include_directories(plugin PUBLIC ${RACK_INCLUDE_DIRS})
target_link_libraries(plugin PUBLIC ${RACK_LIBRARIES})

