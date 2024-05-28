# Automata
Automata is designed to efficiently handle keyboard and mouse input tasks through scripting.
 Developed entirely in C, it ensures high performance and minimal resource usage, with no reliance on external libraries or dependencies.

## Scripting with LUA
Users can write scripts in LUA to define sequences of keyboard and mouse actions.
 The scripting interface exposes functions for key presses, key releases, mouse movements, clicks and more.

Refer to the [scripting documentation](doc/scripting.md) for more details.

### Example

```lua
function hello_world()
  write("hello world")
end

function open_cmd()
  run("powershell.exe")
end

actions = {
  { keycode = { VK_CONTROL, VK_F1 }, action = "hello_world" },
  { keycode = { VK_CONTROL, VK_MENU, VK_T }, action = "open_cmd" },
}
```

# Build

- MSVC
- CMake
- Ninja
- vcpkg; `.\vcpkg install lua:x64-windows-static`

Now you should be able to execute `run.bat`
