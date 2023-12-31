cmake_minimum_required(VERSION 3.10)

project(my_game)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
string(TOLOWER ${CMAKE_BUILD_TYPE} build_dir)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${build_dir})
enable_testing()
message("CMAKE BUILD TYPE = ${CMAKE_BUILD_TYPE}")
LIST(APPEND sources src/Main.cpp 
                    src/Application.cpp
                    src/Logger.cpp
                    src/GameController.cpp
                    src/ResourceManager.cpp
                    src/Game.cpp
                    src/Map.cpp
                    src/MenuController.cpp
                    src/IController.cpp
                    )
foreach(source ${sources})
    message(${source})
endforeach()
add_executable(main ${sources})
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(main BEFORE PRIVATE "./inc")
