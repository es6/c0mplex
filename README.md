# c0mplex
This is an internal cheat for CS:GO created for learning purposes. Utilizes the [MinHook](https://github.com/TsudaKageyu/minhook) API library for hooking into the game's interfaces.

## Features
- Menu based on [ImGui](https://github.com/ocornut/imgui)
- Triggerbot
  - Utilizes TraceRay to create a ray from the local player's eye position to a desired destination
- Perfected Jump
  - Checks for valid player flags and forces jump command
- Radar
  - Iterates through every entity that is a player and marks them as spotted on the in-game mini-map
- Colored Models (Chams)
  - Checks for valid enemies and then overrides their model with a material
- Interface Manager 
  - Automatically captures the game's interface pointers upon game start for easy function hooking
- Network Variable (NetVar) Manager
  - Automates the retrieval of the latest game offsets

## TODO:
- Extra Sensory Perception (ESP) to display player information (health, name, bone matrix, etc.)
- Aimbot with smoothing and visability check
- Backtracking players within a 200ms window
- Skin changer
- Grenade damage prediction
