# Experiments/DWM_WARLOCK/SDL3

Experiment: Seeing how feasible it is to port [DWM_WARLOCK](https://github.com/MEGAHARD-Games/DWM_WARLOCK) to SDL3

## Building

### Linux CLI

1. clone this repo: https://github.com/MEGAHARD-Games/Experiments-DWM_WARLOCK-SDL3.git
2. cd into the cloned dir
3. run the command `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release`
4. run the command `cmake --build build --config Release --parallel`
5. run the command `cd build/Release/` to go into the directory where the library and game were built
6. run the command `LD_LIBRARY_PATH=. ./TryingFoxHunting` to start it, while making sure that it uses the library files in the current directory

### Windows/Linux

1. install CLion: https://www.jetbrains.com/clion/ (it's free (for non-commercial use))
2. start it, and "create a new project from version control"
3. fill in this URL: https://github.com/MEGAHARD-Games/Experiments-DWM_WARLOCK-SDL3.git
4. click "clone" and wait
5. a popup will appear, for build configuration. the default is fine, so click "OK"
6. it should automatically start configuring the cmake project, so wait for it to finish
7. click the run button in the top right corner (green play icon)
