sudo apt-get update && sudo apt-get upgrade
sudo apt-get install libsdl2-dev
g++ src/main.cpp -o bin/ratio-er -lSDL2main -lSDL2 && bin/pratio-er