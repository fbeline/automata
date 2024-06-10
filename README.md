# Automata
Powerful windows automation tool for advanced users.


Automata is designed to efficiently handle keyboard and mouse input tasks through scripting.
It ensures high performance and minimal resource usage, with no reliance on external libraries or dependencies.

## Scripting with LUA
The scripting interface exposes functions for key presses, key releases, mouse movements, clicks and more.

Refer to the [scripting documentation](doc/scripting.md) for more details.

### Example

```lua
function foo()
  write("foo")
end

function bar()
  tap_key(KC_B)
  wait(50)

  press_key(KC_A)
  wait(100)
  release_key(KC_A)
end

function baz()
  pos = mouse_position()
  mouse_move(pos.x + 100, pos.y + 50)
  mouse_click(M_LEFT)
end

function open_cmd()
  run("powershell.exe")
  -- also accepts working directory
  -- run("C:\\Users", "powershell.exe")
end

actions = {
  { keycode = { KC_CONTROL, KC_F1 }, action = "foo" },
  { keycode = { KC_F2 }, action = "bar" },
  { keycode = { KC_F3 }, action = "baz" },
  { keycode = { KC_CONTROL, KC_MENU, KC_T }, action = "open_cmd" },
}
```

# Quick Start

- Double-click to open `automata.exe`.
- Create a new Lua file or use the `default.lua` located in `$HOME\AppData\Roaming\Automata\`.
- Click on the tray icon to open the menu and select the script you want to load.

# UI

![ui](https://github.com/fbeline/automata/assets/5730881/39e1affb-cd3e-402b-b1ae-daeb413e7a94)

1. **Scripts**: List of available scripts.
1. **Reload**: Reloads the active script to apply any changes.
1. **Log**: Shows the most recent log entries.
1. **Log Pressed Keys**: Logs every pressed key, useful for debugging.
1. **Start with windows**: Launches Automata when the computer starts.
1. **Exit**: Exit the application.

# Build

- MSVC
- CMake
- Ninja
- vcpkg; `.\vcpkg install lua:x64-windows-static`

Now you should be able to execute `run.bat`
