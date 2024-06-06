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
  { keycode = { KC_CONTROL, KC_F1 }, action = "hello_world" },
  { keycode = { KC_CONTROL, KC_MENU, KC_T }, action = "open_cmd" },
}
```

# Quick Start

- Double-click to open `automata.exe`.
- Create a new Lua file or use the `default.lua` located in `$HOME\AppData\Roaming\Automata\`.
- The script file must contain at least an empty table named `actions` to be valid.
- Click on the tray icon to open the menu and select the script you want to load.
- If you modify an existing script, click on `reload` to apply the changes.
- For debugging, click on `log`.

# Build

- MSVC
- CMake
- Ninja
- vcpkg; `.\vcpkg install lua:x64-windows-static`

Now you should be able to execute `run.bat`
