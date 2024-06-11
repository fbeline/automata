cmake_minimum_required(VERSION 3.24)
set(CMAKE_C_STANDARD 99)

project(automata
  VERSION 0.1.2
  DESCRIPTION "Automation tool for Windows"
  HOMEPAGE_URL "https://github.com/fbeline/automata")

find_package(Lua REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RESOURCES resource.rc)

add_executable(automata 
  src/main.c
  src/keycode.h
  src/config.c src/config.h
  src/fs.c src/fs.h
  src/log.c src/log.h
  src/process.c src/process.h
  src/action.c src/action.h
  src/keyboard.c src/keyboard.h
  src/mouse.c src/mouse.h
  src/lua_bridge.c src/lua_bridge.h
  src/ui.c src/ui.h

  ${RESOURCES}
)

target_include_directories(automata PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(automata PRIVATE ${LUA_LIBRARIES})
