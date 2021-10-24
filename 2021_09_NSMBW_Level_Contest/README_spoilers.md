# RoadrunnerWMC's Sept. 2021 NSMBW Level Contest Entry (Spoiler-ful Readme)

This is kind of a joke level. The contest theme was "indoors," so naturally, I made the entire level out of doors.

There's also a smaller secondary joke: the level subverts the intended theme by using outdoor backgrounds, because this was actually technically allowed according to the rules. The rules just stated that "at least 50% of the level must take place inside," and defined "inside" to mean "any level with a roof or ceiling;" there was no mention of backgrounds at all.

## Credits (alphabetical order)

* [Chickensaver (Bahlph#0486)](https://github.com/Bahlph) -- debugging help
* [hus](https://www.youtube.com/channel/UCROjh4jG2rsZ0PD0gn2A79w) -- helping with modeling
* Jimbo Slim -- adding bevel effects to door model textures, playtesting
* ThunderVolt (Kiwi) -- playtesting
* Meatball132 -- bouncing ideas off of, playtesting
* ([myself](https://github.com/RoadrunnerWMC), of course)
* [Shibboleet](https://github.com/shibbo) -- debugging help
* [Skawo](https://www.youtube.com/c/Skawo) -- bouncing ideas off of, testing/debugging on hardware, playtesting

## Build instructions

First, follow the build instructions in the "code" directory's readme.

Then, to build the overall release package, install [nsmbpy](https://github.com/RoadrunnerWMC/nsmbpy) and run:

    python3 build.py

The output will be placed in the `dist` directory.

`build.py` offers some command-line options. Run with `--help` or `-h` for details.
