cmake_minimum_required(VERSION 3.24)
project(automata)
set(CMAKE_C_STANDARD 99)

find_package(Lua REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(automata 
  src/main.c
  src/types.h
  src/fs.c src/fs.h
  src/log.c src/log.h
  src/action.c src/action.h
  src/keyboard.c src/keyboard.h
  src/lua_bridge.c src/lua_bridge.h
  src/ui.c src/ui.h
)

target_include_directories(automata PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(automata PRIVATE ${LUA_LIBRARIES})
