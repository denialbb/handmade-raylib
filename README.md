# handmade-raylib

This project is a C++ codebase inspired by Casey Muratori's Handmade Hero series, utilizing the Raylib library for graphics and audio. The goal is to build a foundational understanding of game development from first principles.

## Getting Started

This project uses `make` to build, run, and test. Before you begin, ensure you have `make`, `g++`, and the `raylib` library installed and correctly configured in your environment.

### Building the Game

To compile the project, run the following command:

```sh
make game
```

This will generate an executable file (`game` on Linux, `game.exe` on Windows) in the root directory.

### Building and Running the Game

To compile and immediately run the game, use:

```sh
make run
```

### Running the Tests

To build and run the unit tests, use the following command:

```sh
make tests
```

### Cleaning Up

To remove all generated binaries and build artifacts, run:

```sh
make clean
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## References

- [Handmade Hero episode guide]("https://guide.handmadehero.org/")
- [Raylib Cheatsheet]("https://www.raylib.com/cheatsheet/cheatsheet.html")
