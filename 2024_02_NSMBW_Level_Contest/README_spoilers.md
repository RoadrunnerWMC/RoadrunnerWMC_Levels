# RoadrunnerWMC's Feb. 2024 NSMBW Level Contest Entry -- "Make or Break" (Spoiler-ful Readme)

This is a level about platforms made of ice that break when you stand on them. It's not a very unique interpretation of the contest theme (the winning level, by hayden0729, uses the same idea), but I went a bit farther and made them respawn after a few seconds, as well as interact with enemies properly. Custom code wasn't allowed for this contest, so that was tricky to do with only the original sprites.

Click below to watch a video of the level on YouTube:

[![YouTube video](https://img.youtube.com/vi/TctbpsnyMBY/0.jpg)](https://www.youtube.com/watch?v=TctbpsnyMBY)

## Credits (alphabetical order)

* Luminyx -- playtesting, feedback
* ([myself](https://github.com/RoadrunnerWMC), of course)
* [Skawo](https://www.youtube.com/c/Skawo) -- helping get the ice platform graphics from NSMBU, playtesting, feedback
* ThunderVolt (Kiwi) -- playtesting, feedback

## Build instructions

To build the preprocessed level and the overall release package, install [nsmbpy](https://github.com/RoadrunnerWMC/nsmbpy) and run:

    python3 build.py

The output will be placed in the `dist` directory.

`build.py` offers some command-line options. Run with `--help` or `-h` for details.
