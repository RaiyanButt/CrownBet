## CrownBet Casino
A casino app that lets the user bet crowns on Blackjack, Rocketman, and Slots. At this stage, only slots has been coded. We have the main menu, a playable slots game, the rulesets as well as mock ups for the visuals completed

Compile with: g++ -std=c++17 -Wall -Wextra \
main.cpp button.cpp crash_game.cpp crash_screen.cpp rng.cpp slots_game.cpp slots_render.cpp \
-I/opt/homebrew/opt/sfml@2/include \
-L/opt/homebrew/opt/sfml@2/lib \
-lsfml-graphics -lsfml-window -lsfml-system \
-o casino