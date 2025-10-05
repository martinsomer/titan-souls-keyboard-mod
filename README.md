# Keyboard Mod for Titan Souls
Improved keyboard and mouse support for Titan Souls on PC

## 🎉 v2.0.0 released! Grab it [here](releases)

## Changelog
* v2.0.0
    * Complete re-write of the project
    * Standalone .dll - no drivers needed
    * Mouse capture - support for windowed mode
* v1.0.1
    * Change default hotkey
    * Always enforce mouse radius limit
* v1.0.0
    * Initial release

## Controls
| Action | Description                                                         | Key         |
|--------|---------------------------------------------------------------------|-------------|
| Up     | Hold to move up                                                     | W           |
| Down   | Hold to move down                                                   | S           |
| Left   | Hold to move left                                                   | A           |
| Right  | Hold to move right                                                  | D           |
| Roll   | Press to roll <br> Hold to run                                      | Spacebar    |
| Attack | Hold to aim with mouse <br> Release to fire <br> Hold to call arrow | Left Mouse  |
| Camera | Hold to move camera with mouse <br> Release to re-center            | Right Mouse |

## Usage
* Download the latest [release](releases)
* Extract version.dll into the game folder
* Launch the game

## Compiling
* Install MSVC x86
* Open MSVC Developer Command Prompt
* Run compile.cmd

## Dependencies
* [MinHook](https://github.com/TsudaKageyu/minhook)
* [Dll Proxy](https://github.com/martinsomer/dll-proxy)
* [SDL v2.0.3](https://github.com/libsdl-org/SDL/tree/release-2.0.3)
