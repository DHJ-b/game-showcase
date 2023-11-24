# What is it?

This repository contains simple **pacman-like** game I've built using c++, cmake and sfml.
It serves 3 main purposes:
1) My friend used it to get his credit
2) I can make it presentable and use it as a reference for recruiting
3) I've asked my younger sister to draw graphics for it so that we have something fun but meaningful to do together

All those things could be achieved by making some sort of web-service or something, but it would not be as fun I guess.
Besides the simpler core functionality is the easier it is to polish it and test myself at using tools I'm not that good at 
(multithreading - sure, let's separate graphic thread from game/rules thread; network - it's possible to make online scoreboard and pvp mode, and so far and so on)

Since my friend was in a hurry with his credit and I barely had 2 days to finish this it's a mess as of now, but it's intended to get better along the way.

## How is it organised?

### Directories structure

* `./resources` contains textures/configs/maps/etc.
* `./src` contains source files (.cpp)
* `./inc` contains header files (.hpp)
* `./test` contains test files (.cpp) although there is none for now
* `./build` contains cmake files if you would clone this repo and run `./build.sh`
* `./bin` contains binaries built with `./build.sh`

### Code Overview

#### Helper Classes:

*Logger* is a self-explanatory class I've made because I thought it would be faster 
than using third-party library since I only need very simple functionality.

*Resource Manager* is a class that is used to store and provide shared resources such as Textures, Fonts, Sounds.
The idea was that I would load everything in one place which would help me avoid reading same resource more than once.
And to encapsulate all work with files in one place.

#### Main Functionality:

The core functionality (game rules) are independent of library (except it uses `sf::Vector2` which could be extracted pain-free
but no sprites/textures/sounds/input handles in a game class). It's currently implemented in `game.hpp` and `game.cpp`. It sure
needs to be divided in subcomponents.

Then there are controllers who handle internal state of program (in main menu, paused, changing settings, etc.)
and works with graphics/inputs. It's certainly can be more clearly separated between View and Controller, but as of know View is kind
of integrated part of Controller and not a separated entity. Top-level controller is currently in `controller.[hc]pp` while game
controller that does heavy-lifting is in ~~surprisingly~~ `gamecontroller.[hc]pp`. That's it - so simple.

## How to build?

If sfml is installed on your system and available for cmake then all you have to do is type `./build.sh` in project root directory.
I've managed to install it by running `sudo pacman -S sfml` however it's system-dependent. I'm planning on modifying build scripts
so that they offer to clone and build sfml for you but it is in the future.

## RoadMap

I've not decided what *exactly* I'm going to do next, probably refactoring controllers, rewriting cmake and game class.
When I'll have something updated I would add list item below:


##### SideNotes:
* I'm using english *on purpose*: it allows me to demonstrate my level of language as it is so that whoever
reads it can decide if it's good enough or not. That's why I don't use spellchecker or online translation services here.
