@echo off

echo Running CMake build...
cmake --build build

if errorlevel 1 (
    echo Failed to build project.
    exit /b 1
)

echo Copying compile_commands...
copy .\build\compile_commands.json .

echo Running ...
.\build\automata.exe

if errorlevel 1 (
    echo Failed to run test.
    exit /b 1
)

exit /b 0
