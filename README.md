# GRAZZE — Windows Gaming Companion

A small native Windows desktop utility written in C++.

## Features
- Dark GRAZZE interface
- Live system RAM usage and memory-load display
- Global shortcut: Ctrl + Alt + F1 to show/hide the information panel
- Esc closes the app
- No game process access, memory editing, injection, anti-cheat bypass, or gameplay automation

## Build
Requirements: Windows 10/11 and Microsoft Visual Studio 2022 with the **Desktop development with C++** workload.

1. Open `GRAZZE.cpp` in Visual Studio.
2. Create a new **Windows Desktop Application** project, or add this file to an empty Windows application project.
3. Set the subsystem to **Windows** (not Console).
4. Build using `x64` and `Release`.

You can also compile from a Developer Command Prompt:
```bat
cl /EHsc /DUNICODE /D_UNICODE GRAZZE.cpp /link /SUBSYSTEM:WINDOWS user32.lib gdi32.lib
```
