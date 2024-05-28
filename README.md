# Automata
Automata is designed to efficiently handle keyboard and mouse input tasks through scripting.
 Developed entirely in C, it ensures high performance and minimal resource usage, with no reliance on external libraries or dependencies.

## Scripting with LUA
Users can write scripts in LUA to define sequences of keyboard and mouse actions.
 The scripting interface exposes functions for key presses, key releases, mouse movements, clicks and more.

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

### Documentation

#### press_key
**Description**: Simulates pressing a specified key.

**Syntax**:
```lua
press_key(key)
```

#### release_key
**Description**: Simulates releasing a specified key.

**Syntax**:
```lua
release_key(key)
```

#### tap_key
**Description**: Simulates tapping (pressing and then releasing) a specified key.

**Syntax**:
```lua
tap_key(key)
```

#### write
**Description**: Simulates typing a string of text.

**Syntax**:
```lua
write(text)
```

#### mouse_position
**Description**: Retrieves the current position of the mouse cursor.

**Syntax**:
```lua
x, y = mouse_position()
```

**Returns**:
- x (integer): The X-coordinate of the mouse cursor.
- y (integer): The Y-coordinate of the mouse cursor.

#### mouse_move
**Description**: Moves the mouse cursor to a specified position.

**Syntax**:
```lua
mouse_move(x, y)
```

#### mouse_click
**Description**: Simulates a mouse click at the current position.

**Syntax**:
```lua
mouse_click(button)
```

**Parameters**:
- button: `M_LEFT`, 'M_RIGHT'

#### run
**Description**: Runs an external program.

**Syntax**:
```lua
run(program)
```

**Parameters**:
- `program` (string): The external program to be executed.

**Example**:
```LUA
run("notepad.exe")
```

#### wait
**Description**: Pauses execution for a specified amount of time.

**Syntax**:
```lua
wait(time)
```

**Parameters**:
- `time` (integer): The number of miliseconds to wait.

**Example**:
```LUA
wait(200)
```

# Build

- MSVC
- CMake
- Ninja
- vcpkg; `.\vcpkg install lua:x64-windows-static`

Now you should be able to execute `run.bat`
