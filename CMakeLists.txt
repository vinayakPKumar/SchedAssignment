#name of the title = CMakeLists.txt

project(SchedProject)
set(CMAKE_CXX_STANDARD 11)

add_executable(scheduler mainFile.cpp utils.cpp utils.h headerDefinitions.h)
