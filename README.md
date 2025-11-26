# Handmade Raylib

A C++ game development project inspired by the Handmade Hero series, built using the Raylib library. This project focuses on implementing foundational game systems from scratch, featuring custom tilemap rendering, shader-based visual effects, and unified asset management.

## Features

*   **Tilemap System**: Custom loader for Playscii (`.psci`) maps using JSON, supporting layered rendering and transparency handling.
*   **Visual Effects**:
    *   **CRT Shader**: Lottes-style CRT post-processing for retro visual aesthetics.
    *   **Particle System**: GPU-based particle explosions with distortion, chromatic tinting, and dispersion logic.
*   **Rendering**: Unified texture management allowing sprites and environment layers to share resources.
*   **Asset Management**: Configurable asset loading for audio, fonts, and textures.

## Getting Started

This project uses **CMake** for its build system. Ensure you have `cmake`, a C++ compiler (e.g., `g++`), and `raylib` installed.

### Build and Run

To configure, build, and execute the game in one step:

```sh
cmake -S . -B build && cmake --build build && ./game
```

## Controls

*   **Arrow Keys**: Move Character
*   **Space**: Trigger Particle Explosion Effect
*   **M**: Toggle Audio Mute

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## References

- [Handmade Hero episode guide]("https://guide.handmadehero.org/")
- [Raylib Cheatsheet]("https://www.raylib.com/cheatsheet/cheatsheet.html")