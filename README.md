For linux:

sudo apt-get update && sudo apt-get upgrade
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt install libx11-dev    
g++ src/main.cpp -o bin/prog -lSDL2main -lSDL2 -lSDL2_ttf -L/usr/X11/lib -lX11 && bin/prog
