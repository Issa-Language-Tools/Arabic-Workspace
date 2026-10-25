cmake_minimum_required(VERSION 3.16)
project(StartApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emit build/compile_commands.json so clangd (LSP) knows the include paths.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Runs Qt's meta-object compiler automatically for any class with Q_OBJECT.
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(SQLite3 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/release")

add_executable(workspace
    src/StartApp.cpp
    src/Plumbing/Database/Database.cpp
    src/Plumbing/Database/Helper.cpp
    src/UI/TitleBar/TitleBar.cpp
    src/UI/Editor/Editor.cpp
    src/UI/Editor/NewEntry/NewEntry.cpp
    src/UI/Editor/WordList/WordList.cpp
    src/UI/Editor/EditPopup/EditPopup.cpp
    src/UI/Settings/Settings.cpp
    src/UI/Theme/Theme.cpp
)

target_include_directories(workspace PRIVATE ${CMAKE_SOURCE_DIR}/structs)

target_link_libraries(workspace PRIVATE Qt6::Widgets SQLite3::SQLite3)
