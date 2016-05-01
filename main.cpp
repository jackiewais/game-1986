
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Escenario.h"



using namespace std;
int main( int argc, char* args[] )
{
int width= 600;
int height = 596;
//crea una ventana y muestra una imagen
Escenario* mapa = new Escenario(width,height);
int x = 200;
int y = 500;
//cargo varios backround 
mapa->insertBackgroundObject("sprites/pelota.png", x,y);
mapa->insertBackgroundObject("sprites/pelota.png", 500,500);
mapa->insertBackgroundObject("sprites/pelota.png", 200,2000);
mapa->insertBackgroundObject("sprites/pelota.png", 150,700);
mapa->insertBackgroundObject("sprites/pelota.png", 100,100);
mapa->insertBackgroundObject("sprites/pelota.png", 450,657);
mapa->insertBackgroundObject("sprites/pelota.png", 235,876);

mapa->lunchScreen();





	return 0;
}
