# Scripting

Follows the list of available functions exposed to LUA in order to automate
keyboard and mouse actions.

[List of available keycodes](../src/keycode.h)

## press_key
**Description**: Simulates pressing a specified key.

**Syntax**:
```lua
press_key(key)
```
**Parameters**:
- `key`: [keycodes](../src/keycode.h)

**Example**:
```lua
press_key(KC_F1)
```

## release_key
**Description**: Simulates releasing a specified key.

**Syntax**:
```lua
release_key(key)
```

**Parameters**:
- `key`: [keycodes](../src/keycode.h)

**Example**:
```lua
release_key(KC_CONTROL)
```

## tap_key
**Description**: Simulates tapping (pressing and then releasing) a specified key.

**Syntax**:
```lua
tap_key(key)
```

**Parameters**:
- `key`: [keycodes](../src/keycode.h)

**Example**:
```lua
tap_key(KC_A)
```

## write
**Description**: Simulates typing a string of text.

**WARNING: This function fully supports QWERTY keyboards. You may face errors 
with dual keys symbols on other layouts**

**Syntax**:
```lua
write(text)
```

**Parameters**:
- `text` (string): The string of text to type.

**Example**:
```lua
write("hello world")
```

## mouse_position
**Description**: Retrieves the current position of the mouse cursor.

**Syntax**:
```lua
pos = mouse_position()
-- pos = {x, y}
```

**Returns**:
- x (integer): The X-coordinate of the mouse cursor.
- y (integer): The Y-coordinate of the mouse cursor.

## mouse_move
**Description**: Moves the mouse cursor to a specified position.

**Syntax**:
```lua
mouse_move(x, y)
```

**Parameters**:
- `x` (integer): The X-coordinate to move the mouse cursor to.
- `y` (integer): The Y-coordinate to move the mouse cursor to.

**Example**:
```lua
mouse_move(100, 200)
```

## mouse_click
**Description**: Simulates a mouse click at the current position.

**Syntax**:
```lua
mouse_click(button)
```

**Parameters**:
- button: `M_LEFT`, 'M_RIGHT'

**Example**:
```LUA
mouse_click(M_LEFT)
```

## run
**Description**: Runs an external program.

**Syntax**:
```lua
run(working_dir, command)
-- or
run(command)
```

**Parameters**:
- `working_dir` (string) **OPTIONAL**: The working directory to command execution
- `program` (string): The external program to be executed.

**Returns**:
- 0: success
- 1: error

**Example**:
```LUA
run("C:\\Users", "powershell.exe")

run("notepad.exe")
```

## wait
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

## message_box
**Description**: Show a message box.

**Syntax**:
```lua
message_box(title, message)
```

**Parameters**:
- `title` (string)  : The message box title
- `message` (string): The message box message.

**Example**:
```LUA
message_box("My Title", "hello world!")
```


