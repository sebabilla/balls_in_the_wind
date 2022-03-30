# Balls in the wind

by Sébastien Abilla

GNU/GPL v3 (voir LICENSE)

[Source code](https://github.com/sebabilla/balls_in_the_wind)

#### Video Demo:  <URL HERE>

#### Description:

* Use arrow keys to control the wind and attempt to align the balls in the correct order. 
* Press Enter to Capture the position (in flight okay).
* I've never managed to go till the 6 balls' stage by playing fairly. Will you manage to reach this last stage and clear it? :)​

CS50x's final project. It's coded in C and exported in js/wasm with emscriptem. All the interactions are coded as force vectors applied to the balls. It's inspired by the Nature of Code, excellent Javascript Edutainment on Daniel Shiffman's Coding Train channel.

![Aperçu](res/img_itch.png)

#### Credits

###### Courses and tutorials

* [CS50x](https://cs50.harvard.edu/x/2022) by David J. Malan
* [FormationVideo](https://www.youtube.com/c/Formationvidéo8/) for the SDL by Jason Champagne
* [The Nature of Code 2](https://youtu.be/70MQ-FugwbI) by Daniel Shiffman 
* [Emscripten and SDL 2 Tutorial](https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl-2-tutorial-part-1/) by John Sharp 

###### Sounds

* Sound effects adapted from [LaSonothèque](https://lasonotheque.org/apropos.html) of Joseph Sardin (Free of right)
* Music truncated from [Peer Gynt Suite No. 1, Op. 46 - III](https://musopen.org/) by the Czech National Symphony Orchestra (public domain)

###### Font

* [Onomanopee One](https://fonts.google.com/specimen/Otomanopee+One) by Gutenberg Labo (Open Font License)

#### Files

* main.c - main game loop prepared for alternative compilation by gcc or emcc
* jeu.* - in game mechanics
* vecteur.* - vectors' maths
* physique.* - Newtonian physic, forces -> acceleration -> speed -> position
* affichage.* - display controled using SDL
* musique.* - sounds controled using SDL Mixer
* controle.* - player inputs

#### Directories

* res - files loaded by the game: music and sounds (.ogg), texts in different languages (.txt), font, and image
* balls_in_the_wind - files generated by emcc, index.html simplified to a minimum Javascript canvas

#### Compilation

GNU/Linux running file

```
gcc *.c -Wall -o balls_in_the_wind -lm $(sdl2-config --cflags --libs) -lSDL2_gfx -lSDL2_ttf -lSDL2_mixer
```

Web Assembly compilation and companion files running in a web browser

```
emcc *c -O2 -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file res/ -s USE_SDL_GFX=2 -s USE_SDL_MIXER=2 -s ALLOW_MEMORY_GROWTH -o balls_in_the_wind.html

```
