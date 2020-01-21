Crash Loyal is a clone of a famous moble app game. This project is not to be
used as a comercial product, simply as a teaching tool for 4150 Game AI.

To build and run the project, open a terminal into this root directory and run
the following command:

g++.exe *.cpp -I./include/SDL2 -L./lib -w -lmingw32 -lSDL2main -lSDL2
-lSDL2_image -o crashloyal

This will generate an executable called 'crashloyal' (windows may make it an
exe, linux or mac may not). The executable will initialize the game state
world, a screen as well as begin scanning for use input.

For more details on the graphics/ application library used please check out
the SDL documentation: https://wiki.libsdl.org/FrontPage

For some concrete examples on how to use SDL, please check out Lazy Foo:
http://lazyfoo.net/tutorials/SDL/index.php
